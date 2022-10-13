import View from "./js/view.js";
import Model from "./js/model.js";
import Reports from "./js/controllers/reports.js";
import Sheets from "./js/controllers/sheets.js";

window.addEventListener("DOMContentLoaded", async () => {
  const view = new View();
  const model = new Model();
  const sheets = new Sheets(view, model);
  const reports = new Reports(view, model);

  view.setModel(model);
  model.setView(view);
  model.setReports(reports);
  model.setSheets(sheets);

  reports.onClick((bool) => reports.driveTable(true));
  sheets.onClick((bool) => sheets.driveTable(true));

  document.querySelector("#id").oninput = (e) => model.searchId(e.target.value);

  view.reports = await view.getReports();
  view.sheets = await view.getSheets();

});

