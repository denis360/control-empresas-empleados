import Sheets from "./controllers/sheets.js";
import Filter from "./controllers/filter.js";

export default class Model {
  constructor () {
    this.filters = new Filter();
    this.sheets = new Sheets();
    this.filters.onClick((filters) => this.filter(filters));
    this.btn = document.getElementById("home");
    this.btn.onclick = () => document.getElementById("c-home").classList.remove("d-none");
    this.table = document.getElementById("c-table-body");
    this.objectsReports = null;
  }

  setView (view) {
    return this.view = view;
  }
  
  setReports (reports) {
    this.reports = reports;
  }

  setSheets (sheets) {
    this.sheets = sheets;
  }

  async getFileReport (id) {
    const report = document.getElementById(id).cells[0].innerHTML;
    this.objectReports = await fetch(`/reportes/${report}.html`);
    this.objectReports = await this.objectReports.json();
    this.clearTable();
    this.objectReports.forEach((report) => this.createRowInformation(report));
  }

  async getFileSheet (id) {
    const sheet = document.getElementById(id).cells[0].innerHTML;
    this.objectSheets = await fetch(`/planillas/${sheet}.db`);
    this.objectSheets = await this.objectSheets.json();
    this.clearTable();
    this.objectSheets.forEach((report) => this.createRowInformation(report));
  }

  filter (filters) {
    const { type } = filters;
    for ( const row of this.table.children ) {
      const [, , , status, , , salary] = row.children;
      let shouldHide = false;

      if ( type === "suspends" && status.innerText !== "S" ) {
        shouldHide = true;
      }
      
      if ( type === "minSalary" && Number(salary.innerText) > 2500 ) {
        console.log(status)
        shouldHide = true;
      }

      if ( type === "maxSalary" && Number(salary.innerText) < 3000 ) {
        shouldHide = true;
      }

      if (shouldHide) {
        row.classList.add("d-none");
      } else {
        row.classList.remove("d-none");
      }
    }
  }
  
  searchId (id) {
    const table = document.getElementById("c-table-select").children;
     for ( const row of table ) {
      if (id.length < 8) return;
      const [ name ] = row.children;
      if (name.innerText.split("_")[0] == id) {
        let shouldHide = false;
        if (id) {
          shouldHide = !name.innerText.includes(id);
        }
        if (shouldHide) {      
          row.classList.add("d-none");
        } else {
          row.classList.remove("d-none");
        }
      } else {
        row.classList.add("d-none");
      }
    }    
  }
  
  clearTable () {
    if ( this.table.children.length > 0 ) {
      for (let i = this.table.children.length-1; i>-1; i--) {
        this.table.deleteRow(i);
      }
    }
    return;
  }
  
  createRowPanel (element, id) {
    const row = this.panel.insertRow();
    row.setAttribute("id", id);
    row.classList.add("d-none");
    row.innerHTML = `
      <td>${element}</td>
    `;
    row.onclick = () => this.getFileReport(id);
  }

  createRowInformation (element) {
    const row = this.table.insertRow();
    row.innerHTML = `
      <td>${element.id}</td>
      <td>${element.name}</td>
      <td>${element.job}</td>
      <td>${element.status}</td>
      <td>${element.state}</td>
      <td>${element.stateSalary}</td>
      <td>${element.salary}</td>
      <td>${element.description}</td>
      <td>${element.company}</td>
    `
  }
}
