const router = require("express").Router();
const path = require("path");
const fs = require("fs");

async function getContentReports () {
  const content = { reports: [] };

  const direntReports = await fs.promises.opendir(path.join(process.env.PATH_BASE, "reportes"));

  for await ( element of direntReports ) {
    if (!element.isDirectory()) {
      content.reports.push(element.name.slice(0, -5));
    }
  }

  content.reports.sort();
  return content;
}

router.get("/reports", async (req, res) => {
  const content = await getContentReports()
  res.render("reports", { content });
});

router.get("/reports/name=:name", async (req, res) => {
  const content = await getContentReports()
  const { name } = req.params;
  let html = await fs.readFileSync(path.join(process.env.PATH_BASE, "reportes", name+".html"), "utf-8");
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

  res.render("reports", { content, reports, name });

});

module.exports = router;
