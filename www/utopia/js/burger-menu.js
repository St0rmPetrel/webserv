let burger = document.querySelector('.header__burger');
let nav = document.querySelector('.header__nav');
let body = document.querySelector('body');

burger.onclick = function () {
    burger.classList.toggle('active');
    nav.classList.toggle('active');
    body.classList.toggle('lock');
};