const router = require("express").Router();
const path = require("path");
const fs = require("fs");

async function getContentSheets (id) {
  let content = { sheets: [] }

  const direntSheets = await fs.promises.opendir(path.join(process.env.PATH_BASE, "planillas"));

  for await ( element of direntSheets ) {
    if (!element.isDirectory()) {
      let id_ = element.name.split("_")[0];
      if (id_ == id) {
        content.sheets.push(element.name.slice(0, -3).slice(id.length+1, this.length));
      }
    }
  }
  content.sheets.sort();

  return content;
}

router.get("/sheets", async (req, res) => {
  res.render("sheets");
});

router.get("/sheets/:id", async (req, res) => {
  const { id } = req. params;
  const content = await getContentSheets(id)
  res.render("sheets", { content, id})
});

router.get("/sheets/:id/name=:name", async (req, res) => {
  const content = await getContentSheets(req.params.id);
  const { name } = req.params;
  let sheet = await fs.readFileSync(path.join(process.env.PATH_BASE, "planillas", name+".db"), "utf-8");
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
  res.render("sheets", { content, name, sheets, id: req.params.id })
})

router.post("/sheets/company", async (req, res) => {
  const { id_company } = req.body;
  let companies = await fs.readFileSync(path.join(process.env.PATH_BASE, "bases-datos", "empresas.db"), "utf-8");
  companies = companies.toString().replace(/(\n)/gm, "_").split("_").filter(Boolean);
  for ( let it = 0; it<companies.length; it+=4 ) {
    if ( id_company == companies[it] ) {
      return res.redirect(`/sheets/${id_company}`);
      break;
    }
  }
  return res.render("sheets", { alert: "La empresa no existe" });
});

module.exports = router;
