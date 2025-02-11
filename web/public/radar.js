import { Application, Assets, Sprite, Graphics } from 'pixi.js';
import { io } from 'socket.io-client';

const socket = io("ws://192.168.0.23:3000");

let app;
let currentMap = "";
let playerSprites = {};

const container = document.getElementById("map");

app = new Application();
await app.init({
    width: container.clientWidth,
    height: container.clientHeight,
    backgroundColor: 0x111111,
    resolution: window.devicePixelRatio || 1,
    autoDensity: true,
});

container.appendChild(app.canvas);

const MAP_CONFIG = {
    "cs_italy":     { xRef: -2647, yRef: 2592, scale: 4.6 },
    "cs_office":    { xRef: -1838, yRef: 1858, scale: 4.1 },
    "de_mirage":    { xRef: -3230, yRef: 1713, scale: 5.0 },
    "de_dust2":     { xRef: -2476, yRef: 3239, scale: 4.4 },
    "de_inferno":   { xRef: -2087, yRef: 3870, scale: 4.9 },
    "de_overpass":  { xRef: -4831, yRef: 1781, scale: 5.2 },
    "de_anubis":    { xRef: -2796, yRef: 3328, scale: 5.22 },
    "de_vertigo":   { xRef: -3168, yRef: 1762, scale: 4 },
    "de_ancient":   { xRef: -2953, yRef: 2164, scale: 5 },
    "de_nuke":      { xRef: -3453, yRef: 2887, scale: 7 },
};

const PLAYER_COLOR = {
    blue:   0x6ba0dc,
    purple: 0x7c037e,
    yellow: 0xfafd06,
    orange: 0xfe5807,
    green:  0x04a151,
    red:    0xff0000
}

function getPlayerPositionFromOrigin(x, y, mapName) {
    if (!MAP_CONFIG[mapName] || !app.stage.children[0])
        return { x: 512, y: 512 };

    const { xRef, yRef, scale } = MAP_CONFIG[mapName];

    let mapX = (x - xRef) / scale;
    let mapY = (yRef - y) / scale;

    const mapSprite = app.stage.children[0];
    const scaleFactor = mapSprite.scale.x;

    mapX = (mapX * scaleFactor) + mapSprite.x;
    mapY = (mapY * scaleFactor) + mapSprite.y;

    return { x: Math.round(mapX), y: Math.round(mapY) };
}

function getPlayerColorFromOrigin(originColor, originTeam, playerID) {
    if (playerID === 0)
        getPlayerColorFromOrigin.localTeam = originTeam;

    if (originTeam !== getPlayerColorFromOrigin.localTeam)
        return PLAYER_COLOR.red;

    switch (originColor) {
        case 0:  return PLAYER_COLOR.blue;
        case 1:  return PLAYER_COLOR.green;
        case 2:  return PLAYER_COLOR.yellow;
        case 3:  return PLAYER_COLOR.orange;
        case 4:  return PLAYER_COLOR.purple;
        default: return "#ffffff";
    }
}

async function loadMap(mapName) {
    const texture = await Assets.load(`http://192.168.0.23:3000/map/${mapName}.png`);

    if (app.stage.children.length > 0) {
        app.stage.removeChildren();
    }

    const mapSprite = new Sprite(texture);

    const originalWidth = texture.width;
    const originalHeight = texture.height;

    const containerWidth = app.renderer.width;
    const containerHeight = app.renderer.height;

    const scale = Math.min(containerWidth / originalWidth, containerHeight / originalHeight);
    mapSprite.scale.set(scale);

    mapSprite.x = (containerWidth - originalWidth * scale) / 2;
    mapSprite.y = (containerHeight - originalHeight * scale) / 2;

    app.stage.addChild(mapSprite);
}


socket.on("mapUpdate", (data) => {
    console.log(data);
    if (currentMap !== data.mapName) {
        currentMap = data.mapName;
        loadMap(currentMap);
    }
});

socket.on("playersUpdate", (player_list) => {
    player_list.forEach(player => {
        let { x, y } = getPlayerPositionFromOrigin(player.position.x, player.position.y, currentMap);
        let color = getPlayerColorFromOrigin(player.color, player.team, player.id);

        if (!playerSprites[player.name]) {
            let playerSprite = new Graphics()
                .circle(0, 0, 5)
                .fill(color);

            app.stage.addChild(playerSprite);
            playerSprites[player.name] = playerSprite;
        }

        playerSprites[player.name].x = x;
        playerSprites[player.name].y = y;
    });
});
