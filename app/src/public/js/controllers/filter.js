export default class Filter {
  constructor () {
    this.form = document.getElementById("filters");
    this.form.classList.remove("d-none");
    this.btn = document.getElementById("form-search");
  }
  onClick (callback) {
    this.btn.onclick = (e) => {
      e.preventDefault();
      const data = new FormData(this.form);
      callback({
        "type": data.get("type")
      })
    }
  }
}
