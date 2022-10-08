const router = require("express").Router();
const path = require("path");
const fs = require("fs");

async function getContentReports (id) {
  const content = { reports: [] };

  const direntReports = await fs.promises.opendir(path.join(process.env.PATH_BASE, "reportes"));

  for await ( element of direntReports ) {
    if (!element.isDirectory()) {
      let id_ = element.name.split("_")[0];
      if (id_ == id) {
        content.reports.push(element.name.slice(0, -5));
      }
    }
  }

  content.reports.sort();
  return content;
}

router.get("/reports", async (req, res) => {
  res.render("reports");
});

router.get("/reports/:id", async (req, res) => {
  const { id } = req. params;
  const content = await getContentReports(id)
  res.render("reports", { content, id})
});

router.get("/reports/:id/name=:name", async (req, res) => {
  const content = await getContentReports(req.params.id)
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

  res.render("reports", { content, reports, name, id: req.params.id });

});

router.post("/reports/company", async (req, res) => {
  const { id_company } = req.body;
  let companies = await fs.readFileSync(path.join(process.env.PATH_BASE, "bases-datos", "empresas.db"), "utf-8");
  companies = companies.toString().replace(/(\n)/gm, "_").split("_").filter(Boolean);
  for ( let it = 0; it<companies.length; it+=4 ) {
    if ( id_company == companies[it] ) {
      return res.redirect(`/reports/${id_company}`);
      break;
    }
  }
  return res.render("reports", { alert: "La empresa no existe" });
});

module.exports = router;
