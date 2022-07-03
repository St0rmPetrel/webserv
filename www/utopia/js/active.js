function shineLinks(id){
        let el=document.getElementById(id).getElementsByTagName('a');
        let url=document.location.href;
        for(let i=0;i<el.length; i++){
            if (url==el[i].href){
                el[i].className += ' act';
            };
        };
};