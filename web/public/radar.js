const socket = io("ws://localhost:3000");

let map;
let current_map_name = "";
let player_markers = {};

function get_player_position_from_origin(x, y, map_name) {

    // ; https://www.unknowncheats.me/forum/counter-strike-2-a/603051-map-radar-images.html
    const map_config = {
        "mirage":   { xRef: -3230, yRef: 1713, scale: 5.0 },
        "dust2":    { xRef: -2476, yRef: 3239, scale: 4.4 },
        "inferno":  { xRef: -2087, yRef: 3870, scale: 4.9 },
        "overpass": { xRef: -4831, yRef: 1781, scale: 5.2 },
    };

    if (!map_config[map_name]) {
        return { x: 512, y: 512 };
    }

    const { xRef, yRef, scale } = map_config[map_name];

    let map_x = (x - xRef) / scale;
    let map_y = (yRef - y) / scale;

    map_y = 1024 - map_y;

    return { x: Math.round(map_x), y: Math.round(map_y) };
}

function get_player_color_from_origin(origin_color, origin_team, player_id) {

    const blue = "#6ba0dc";
    const purple = "#7c037e";
    const yellow = "#fafd06";
    const orange = "#fe5807";
    const green = "#04a151";
    const red = "#f00608";

    // `player_id === 0` is always our player
    if (player_id === 0) {
        get_player_color_from_origin.local_team = origin_team;
    }

    if (origin_team !== get_player_color_from_origin.local_team) {
        return red;
    }

    switch (origin_color) {
        case 0: return blue;
        case 1: return green;
        case 2: return yellow;
        case 3: return orange;
        case 4: return purple;
        default: return "#ffffff";
    }
}

socket.on("map_update", (data) => {

    if (current_map_name !== data.map_name) {
        current_map_name = data.map_name;

        if (map) {
            map.remove();
        }

        map = L.map('map', {
            crs: L.CRS.Simple,
            minZoom: -2,
            maxZoom: 2,
            zoomControl: false
        });

        map.getContainer().style.background = "#111";

        let bounds = [[0, 0], [1024, 1024]];
        L.imageOverlay(`/map/${current_map_name}.png`, bounds).addTo(map);
        map.fitBounds(bounds);

        player_markers = {};
    }
});

socket.on("players_update", (player_list) => {

    player_list.forEach(player => {
        let { x, y } = get_player_position_from_origin(
                                            player.position.x,
                                            player.position.y,
                                            current_map_name);

        if (player_markers[player.name]) {
            map.removeLayer(player_markers[player.name]);
        }

        if (player.is_alive) {
            player_markers[player.name] = L.circleMarker([y, x], {
                radius: 4,
                color: get_player_color_from_origin(player.color, player.team, player.id),
                fillOpacity: 1
            }).addTo(map);
        } else {
            player_markers[player.name] = L.marker([y, x], {
                icon: L.divIcon({
                    className: "dead-player",
                    html: `<span style="color: ${get_player_color_from_origin(player.color, player.team, player.id)};">&#10007;</span>`,
                    iconSize: [20, 20], 
                    iconAnchor: [7, 7],
                })
            }).addTo(map);
        }

        player_markers[player.name].bindTooltip(player.name, {
            permanent: true,
            direction: "center",
            offset: [0, -15],
            className: "player",
            interactive: false
        }).openTooltip();
    });
});

get_player_color_from_origin.local_team = null;
