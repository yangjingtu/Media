// JavaScript Document

function test()
{
	ocx.test();
}

function openMedia( mediaPath )
{
	var isOpen = ocx.OpenMedia(mediaPath, 0);
}

function closeMedia()
{
	var isClose = ocx.CloseMedia();
}

//play video
function play()
{	
	//var url = "ip=192.168.1.218&port=55555&url=D:\\test1.avi";
	var url = "ip=127.0.0.1&port=55555&url=E:\\video\\avi\\20121220_161400.AVI";
	//var url = "D:\\test1.avi";
	var i = 0;
	for( ; i < 1; ++i)
	{
		ocx.SetSelectWnd(i);
		openMedia(url);
		ocx.Play();
	}
	
	//var isPlay = ocx.Play();
}

function pause()
{
	var isPause = ocx.Pause();
}

function stop()
{
	var isStop = ocx.Stop();
}

var forwardSpeed = 1;
function forward(nSpeed)
{
	if(forwardSpeed++ > 4)
	{
		forwardSpeed = 1;
	}
	var isOk = ocx.Forward(forwardSpeed);
}

var reverseSpeed = 1;
function reverse(nSpeed)
{
	if(reverseSpeed++ > 4)
	{
		reverseSpeed = 1;
	}
	var isOk = ocx.Reverse(reverseSpeed);
}

function preframe()
{
	ocx.PreFrame();
}

function nextframe()
{
	ocx.NextFrame();
}

function seek(pos, minVal, maxVal)
{
	ocx.Seek(pos, minVal, maxVal);
}

function setWndNum(count)
{
	if(count < 0 || count >= 9){
		alert("please set value in [0-9]");
		return;
	}
    ocx.SetWndStyle(0, count);
}

function fullscreen()
{
	ocx.FullScreen();
}
 
function snap(path)
{
	ocx.Snap( path );
}