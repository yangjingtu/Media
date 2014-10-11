
var sPosition;
var showPerc = 100;
document.onmousedown = mouseDown
document.onmouseup = new Function("dragMe=false")

var dragMe = false, kObj, xPos, direction
function moveLayer() {
    if (event.button == 1 && dragMe) {
        oldX = kObj.style.pixelLeft; kObj.style.pixelLeft = temp2 + event.clientX - xPos;
        if (kObj.style.pixelLeft > oldX) direction = "right"; else direction = "left";
        if (kObj.style.pixelLeft < 2 && direction == "left") { kObj.style.pixelLeft = 2; direction = "right"; }

        var width = document.getElementById("barLyr").offsetWidth;   //获取表格宽度

        if (kObj.style.pixelLeft > width && direction == "right") { kObj.style.pixelLeft = width; direction = "left"; }
        sPosition = kObj.style.pixelLeft; showPerc = sPosition - 2;

        document.getElementById("time").innerText = sPosition;

        return false;
    }
}

//鼠标按下事件
function mouseDown()
{
	dragLayer();
	clickLayer();
}

//水平拉动
function dragLayer() {
	if (!document.all) return;
    if (event.srcElement.className == "drag") {
        dragMe = true; kObj = event.srcElement; temp2 = kObj.style.pixelLeft; xPos = event.clientX; document.onmousemove = moveLayer;
    }
	
}

//单击移动
function clickLayer() 
{
	if (!document.all) 
		return;
	
	var elemId = event.srcElement.id;
    if (elemId == "innerLyr" || elemId == "barLyr"|| elemId == "outerLyr") {
		var dragObj = document.getElementById("knobImg");
		var val =  event.offsetX;
		var width = document.getElementById("barLyr").offsetWidth;   //获取宽度
		if( val > width ) val = width;
		if( val < 2 ) val = 2;
		
		dragObj.style.pixelLeft = val;
		
        document.getElementById("time").innerText = val;
		
		seek(val, 2, width);
	}
}

function setLayerPos(pos) 
{
	if (!document.all) 
		return;
		
	var dragObj = document.getElementById("knobImg")
	var width = document.getElementById("barLyr").offsetWidth;   //获取宽度
	if( pos > width ) pos = width;
	if( pos < 2 ) pos = 2;
	
	dragObj.style.pixelLeft = pos;
	
	seek(pos, 2, width);
}