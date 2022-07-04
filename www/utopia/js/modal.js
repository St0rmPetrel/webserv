let modal = document.getElementById('modal');
let bShow = document.getElementsByClassName('download-button');
let span = document.getElementsByClassName("close-cross")[0];


for (let i = 0; i < bShow.length; i++) {
    bShow[i].onclick = function() {
        modal.style.display = "block";
        // body.classList.toggle('lock');
    }
  }

span.onclick = function() {
    modal.style.display = "none";
}
        
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}