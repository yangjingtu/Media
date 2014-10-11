var gPlugin;		//插件对象
var gConn;

// JavaScript Document
function WebInit(DownPluginTip, ie){
	try{
		gPlugin = new classMedia();
		if (ie){ 
			gPlugin.obj = document.getElementsByTagName("object");
			gPlugin.index = 0;
			
			if($.browser.version != "6.0"){
				//window.onmousewheel= document.onmousewheel =function(){return false}
			}
		}
		else{ gPlugin.obj = document.getElementsByTagName("Embed");
			gPlugin.Test();
		}
		
		SetConnInfo('127.0.0.1', 8080, 'test', 'test', 0, 0);
		
		OnLogin();
	}
	catch(e)
	{
		
	}
}

function SetConnInfo(ip, port, user, pwd, type, n)
{
	if( !gConn )
	{	
		gConn = new ConnInfo();
	}
	gConn.ip = ip;
	gConn.port = port;
	gConn.user = user;
	gConn.pwd = pwd
	gConn.type = type;
	gConn.n = n;
}

function OnLogin()
{
	gPlugin.Login(gConn.ip, gConn.port, gConn.port, gConn.user, 
			gConn.pwd, gConn.type, gConn.n);
}

function OnPlay()
{
	gPlugin.VedioPlay(0, '127.0.0.1', 3);
}