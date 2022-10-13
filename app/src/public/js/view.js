export default class View {
  constructor () {
    this.reports = null;
    this.sheets = null;
    this.btn = document.getElementById("home");
    this.containerTable = document.getElementById("c-table");
    this.btn.onclick = () => this.tablesHideShow();
  }

  async getReports () {
    let reports = await fetch("/reportes");
    reports = await reports.json();
    return reports.reports;
  }

  async getSheets () {
    let sheets = await fetch("/planillas");
    sheets = await sheets.json();
    return sheets.sheets;
  }

  setModel (model) {
    this.model = model;
  }

  tablesHideShow () {
    document.getElementById("c-home").classList.remove("d-none");
    this.containerTable.add("d-none");
  }
}
