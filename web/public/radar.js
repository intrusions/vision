const socket = io("ws://localhost:3000");

let map;
let current_map_name = "";
let playerMarkers = {};

// ; https://www.unknowncheats.me/forum/counter-strike-2-a/603051-map-radar-images.html
const mapConfig = {
    "mirage":   { xRef: -3230, yRef: 1713, scale: 5.0 },
    "dust2":    { xRef: -2476, yRef: 3239, scale: 4.4 },
    "inferno":  { xRef: -2087, yRef: 3870, scale: 4.9 },
    "overpass": { xRef: -4831, yRef: 1781, scale: 5.2 },
};

function convertPositionToMap(x, y, mapName) {
    if (!mapConfig[mapName]) {
        return { x: 512, y: 512 };
    }

    const { xRef, yRef, scale } = mapConfig[mapName];

    let mapX = (x - xRef) / scale;
    let mapY = (yRef - y) / scale;

    mapY = 1024 - mapY;

    return { x: Math.round(mapX), y: Math.round(mapY) };
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

        playerMarkers = {};
    }
});

socket.on("players_update", (player_list) => {
    player_list.forEach(player => {
        let { x, y } = convertPositionToMap(player.position.x, player.position.y, current_map_name);

        if (playerMarkers[player.name]) {
            playerMarkers[player.name].setLatLng([y, x]);
        } else {
            playerMarkers[player.name] = L.circleMarker([y, x], {
                radius: 6,
                color: player.name === "xel" ? "#FF0000" : "#00FF00",
                fillOpacity: 1
            }).addTo(map);
        }
    });
});

