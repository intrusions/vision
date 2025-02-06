const net = require("net");
const express = require("express");
const { Server } = require("socket.io");
const http = require("http");
const path = require("path");

let current_map_name = "default";

/*
* @Brief Creat a local web server on port 3000
* then manage map state.
*/
const app = express();
const web_server = http.createServer(app);
const io = new Server(
    web_server,
    { cors: { origin: "*" } }
);

io.on("connection", (socket) => {
    console.log("web client connected");

    socket.emit("map_update", { map_name: current_map_name });

    socket.on("disconnect", () => {
        console.log("web client disconnected");
    });
});

app.use(express.static("public"));
app.use("/map", express.static(path.join(__dirname, "map")));

web_server.listen(3000, "0.0.0.0", () => {
    console.log("web server successfully started: http://127.0.0.1:3000");
});


/*
* @Brief Creat a local TCP server on port 4000,
* manage clients connection/disconnection and data reception.
* This server is used by cheat itself to send players data.
*/
const tcp_server = net.createServer((socket) => {
    console.log("usermode client connected");

    socket.on("data", (data) => {
        const json_data = JSON.parse(data.toString("utf8"));

        if (json_data.map_name !== current_map_name) {
            current_map_name = json_data.map_name;
            io.emit("map_update", { map_name: current_map_name });
        }

        io.emit("players_update", json_data.players);
    });

    socket.on("end", () => {
        console.log("usermode client disconnected");
    });

    socket.on("error", (err) => {
        console.error("socket error:", err.message);
    });
});

tcp_server.listen(4000, "0.0.0.0", () => {
    console.log("tcp server successfully started: http://127.0.0.1:4000");
});
