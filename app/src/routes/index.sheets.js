const router = require("express").Router();
const path = require("path");
const fs = require("fs");

async function getContentSheets () {
  let content = { sheets: [] }

  const direntSheets = await fs.promises.opendir(path.join(process.env.PATH_BASE, "planillas"));

  for await ( element of direntSheets ) {
    if (!element.isDirectory()) {
      content.sheets.push(element.name.slice(0, -3));
    }
  }
  content.sheets.sort();

  return content;
}

router.get("/sheets", async (req, res) => {
  const content = await getContentSheets();
  res.render("sheets", { content });
});

router.get("/sheets/name=:name", async (req, res) => {
  const content = await getContentSheets();
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
  res.render("sheets", { content, name, sheets })
})

module.exports = router;
