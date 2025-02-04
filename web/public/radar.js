const socket = io("ws://localhost:3000");
let map;
let current_map_name = "";

socket.on("map_update", (data) => {
    if (current_map_name !== data.map_name) {
        current_map_name = data.map_name;
        
        if (map) {
            map.remove();
        }
    
        document.getElementById("map").innerHTML = "";
        map = L.map(
            'map',
            {
                crs: L.CRS.Simple,
                minZoom: -2,
                maxZoom: 2,
                zoomControl: false
            }
        );
        map.getContainer().style.background = "#111";
    
        let bounds = [[0, 0], [1024, 1024]];
        L.imageOverlay("/map/" + current_map_name + ".png", bounds).addTo(map);
        map.fitBounds(bounds);
    }
});

socket.on("players_update", (data) => {
    console.log(data);
});