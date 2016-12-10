$( document ).ready(function() {
    var scaleNumSlide=function(){
        var height=$(document).height();
        if(height>50)
            return (height/768);
        else
            return 1;
    }
    setTimeout(function(){
        var slides=$(".slides")[0].children;
        var isClass=function(elementNode,classElement){
            for(var i=0;i<elementNode.classList.length;i++){
                if(elementNode.classList[i]==classElement) return true;
            }
            return false;
        };
        var number=1
        for(var i=0;i<slides.length;i++){        
            if(isClass(slides[i],"stack")){
                var childrensOfstackSlide=slides[i].children;
                for(var j=1;j<childrensOfstackSlide.length;j++){
                    childrensOfstackSlide[j].innerHTML=childrensOfstackSlide[j].innerHTML+'<div class="enumDiv" style="visibility: hidden; position:fixed; float:left;">'+number+'</div>';
                    number=number+1;
                }            
            }
        }
        functionsVectorForExecuteAfterStartEmbeddedContent.push(
            function(slide){
                var childrensOfSlide=slide.children;
                var childrensOfSlideHaveEnumDiv=false;
                for(var j=1;j<childrensOfSlide.length;j++){
                    if(isClass(childrensOfSlide[j],"enumDiv")){
                        childrensOfSlideHaveEnumDiv=true;                
                        $("#numOfTheSlide").html(childrensOfSlide[j].innerHTML);
                        break;
                    }
                }
                if(childrensOfSlideHaveEnumDiv==false){
                    $("#numOfTheSlide").html("");

                }           
            }

        );
        $(document).resize(function() {    
            $("#numOfTheSlide").css("font-size",parseInt(Math.round(36*scaleNumSlide()),10)+"px");
        });
        $( '<div id="numOfTheSlide" class=".reveal" style="font-family: Lato, sans-serif; font-size: 18px; position: fixed; bottom: 40px; right: 40px;"></div>' ).insertAfter(".controls");
        setTimeout(function (){ $("#numOfTheSlide").css("font-size",parseInt(Math.round(36*scaleNumSlide()),10)+"px");},1300);
        setTimeout(function (){ $("#numOfTheSlide").css("font-size",parseInt(Math.round(36*scaleNumSlide()),10)+"px");},1300);

    },800);
});
