const net = require("net");
const express = require("express");
const { Server } = require("socket.io");
const http = require("http");
const path = require("path");
const cors = require("cors");

let currentMapName = "default";

const app = express();
app.use(cors());

const webServer = http.createServer(app);
const io = new Server(webServer, {
    cors: {
        origin: "*",
        methods: ["GET", "POST"]
    }
});

io.on("connection", (socket) => {
    console.log("Web client connected");
    socket.emit("mapUpdate", { mapName: currentMapName });

    socket.on("disconnect", () => {
        console.log("Web client disconnected");
    });
});

app.use("/map", express.static(path.join(__dirname, "map")));
app.use("/modules", express.static(path.join(__dirname, "node_modules")));
app.use("", express.static(path.join(__dirname, "public")));

webServer.listen(3000, "0.0.0.0", () => {
    console.log("Web server successfully started: http://127.0.0.1:3000");
});

const tcpServer = net.createServer((socket) => {
    console.log("Usermode client connected");

    socket.on("data", (data) => {
        try {
            const jsonData = JSON.parse(data.toString("utf8"));
            if (jsonData.map_name !== currentMapName) {
                currentMapName = jsonData.map_name;
                io.emit("mapUpdate", { mapName: currentMapName });
            }
            
            io.emit("playersUpdate", jsonData.players);
        } catch (error) {
            console.error("JSON parsing error:", error.message);
        }
    });

    socket.on("end", () => {
        console.log("Usermode client disconnected");
    });

    socket.on("error", (err) => {
        console.error("Socket error:", err.message);
    });
});

tcpServer.listen(4000, "0.0.0.0", () => {
    console.log("TCP server successfully started: http://127.0.0.1:4000");
});
