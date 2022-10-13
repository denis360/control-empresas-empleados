const router = require("express").Router();
const path = require("path");
const fs = require("fs");


router.get("/planillas", async (req, res) => {
  const content = { sheets: [] };
  const dirent = await fs.promises.opendir(path.join(process.env.BASE_PATH, "planillas"));
  for await (let element of dirent) {
    if (!element.isDirectory()) {
      content.sheets.push(element.name);
    }
  }
  content.sheets.sort();
  res.status(200).json(content);
});

router.get("/reportes", async (req, res) => {
  const content = { reports: [] };
  const dirent = await fs.promises.opendir(path.join(process.env.BASE_PATH, "reportes"));
  for await (let element of dirent) {
    if (!element.isDirectory()) {
      content.reports.push(element.name);
    }
  }
  content.reports.sort();
  res.status(200).json(content);
});

router.get("/planillas/:file", async (req, res) => {
  const { file } = req.params;
  let sheet = await fs.readFileSync(path.join(process.env.BASE_PATH, "planillas", file), "utf-8");
  sheet = sheet.toString()
    .replace(/(\n)/gm, "_")
    .split("_")
    .filter(Boolean);
  let sheets = [];
  for ( let i = 0; i<sheet.length; i+=9 ) {
    let newSheet = {};
    newSheet.id = sheet[i];
    newSheet.name = sheet[i+1];
    newSheet.job = sheet[i+2];
    newSheet.status = sheet[i+3];
    newSheet.state = sheet[i+4];
    newSheet.stateSalary = sheet[i+5];
    newSheet.salary = sheet[i+6];
    newSheet.description = sheet[i+7];
    newSheet.company = sheet[i+8];
    sheets.push(newSheet);
  }
  
  res.status(200).json(sheets);
});

router.get("/reportes/:file", async (req, res) => {
  const { file} = req.params;
  let html = await fs.readFileSync(path.join(process.env.BASE_PATH, "reportes", file), "utf-8");
  html = html.toString()
    .replace(/(<[^>]+>)/ig, '')
    .replace(/(\n)/gm, "_")
    .split("_")
    .filter(Boolean);
  html = html.slice(10, html.length);
  let reports = [];

  for (let i = 0; i<html.length; i+=9 ) {
    let report = {}
    report.id = html[i];
    report.name = html[i+1];
    report.job = html[i+2];
    report.status = html[i+3];
    report.state = html[i+4];
    report.stateSalary = html[i+5];
    report.salary = html[i+6];
    report.description = html[i+7];
    report.company = html[i+8];
    reports.push(report);
  }
  
  res.status(200).json(reports);
});

module.exports = router;
