const router = require("express").Router();
const path = require("path");
const fs = require("fs");

router.use(require("./index.reports"));
router.use(require("./index.sheets"));

router.get("/", async (req, res) => {
  res.render("index");
});

module.exports = router;
