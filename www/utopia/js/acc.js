let acc = document.getElementsByClassName("faq__accordion");
let i;

function blockHide(){
    panel.style.display = "none";
}

for (i = 0; i < acc.length; i++) {
    acc[i].addEventListener("click", function() {
        this.classList.toggle("active");

        let panel = this.nextElementSibling;
        if (panel.style.display === "block") {
            // block.classList.add('anime');
            // blockHide();
            panel.style.display = "none";
        } else {
            panel.style.display = "block";
        }
    });
}