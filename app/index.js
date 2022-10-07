require("dotenv").config();
const express = require("express");
const exphbs = require("express-handlebars");
const path = require("path");

// initializations
const app = express();

// settings
app.set("port", process.env.PORT || 3000);
app.set("views", path.join(__dirname, "src", "views"));
app.engine(".hbs", exphbs.engine({
  defaultLayout: "main",
  layoutsDir: path.join(app.get("views"), "layouts"),
  partialsDir: path.join(app.get("views"), "partials"),
  extname: ".hbs"
}));
app.set("view engine", ".hbs");

// routes
app.use(require("./src/routes/index.router"));

// starting the server
app.listen(app.get("port"), () => console.log("Server on port", app.get("port")));

