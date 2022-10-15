export default class Reports {
  constructor (view, model) {
    this.view = view;
    this.model = model;
    this.btn = document.querySelector("#reports");
    this.containerHome = document.getElementById("c-home");
    this.containerTable = document.getElementById("c-table");
    this.panel = document.getElementById("c-table-select");
  }
  
  driveTable (bool) {
    this.showPanelToSelect([...this.view.reports]);
    if (bool) {
      this.containerTable.classList.remove("d-none"); 
      this.containerHome.classList.add("d-none"); 
    } else {
      if (!bool) {      
        this.containerTable.classList.add("d-none");
        this.containerHome.classList.remove("d-none"); 
      }
    }
  }
  
  showPanelToSelect (object) {
    document.getElementById("tag").innerText = "Reportes";
    this.model.clearTable();
    const table = document.getElementById("c-table-select");
    if ( table.children.length > 0 ) {
      for (let i = table.children.length-1; i>-1; i--) {
        table.deleteRow(i);
      }
    }
    let i = 0;
    object.forEach((e) => {
      this.createRowPanel(e.split(".")[0], i+=1);
    });
  }

  createRowPanel (element, id) {
    const row = this.panel.insertRow();
    row.setAttribute("id", id);
    row.innerHTML = `
      <td>${element}</td>
    `;
    row.onclick = () => this.model.getFileReport(id);
  }

  onClick (callback) {
    this.btn.onclick = () => {
      document.getElementById("filters").classList.remove("d-none");
      document.getElementById("input-id").classList.add("d-none");
      document.getElementById("id").value = "";
      callback(true);
    }
  }
}
