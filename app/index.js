// import dependences
require("dotenv").config();
const express = require("express");
const path = require("path");

// initializations
const app = express();

// settings
app.set("port", process.env.PORT || 3000);

// middleware
app.use(express.urlencoded({ extended: true }));
app.use(express.json());

// routes
app.use(require("./src/routes/index.router.js"));

// static files
app.use(express.static(path.join(__dirname, "src", "public")));

// start the server
app.listen(app.get("port"), () => console.log("Server on port", app.get("port")));

