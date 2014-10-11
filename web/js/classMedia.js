// JavaScript Document

function classMedia(){
	this.obj = null;	//控件对象
	this.index = 0;		//控件索引
	
	this.Test = function(){return this.obj[0].test();}
	this.Login = function(ip, port, webport, user, pass, streamType,n){
		this.obj[0].Login(ip, port, webport, user, pass, streamType,n);
	}
	
	this.FullScreen=function(){
		this.obj[IFs].FullScreen();
	}
	this.SetLauguage=function(n){
		for(var i=0; i<9; i++)
		this.obj[i].SetLauguage(n);
	}
	this.Record=function(t){
		/*var c=getCookie("Record"+IFs);
		if(c==null){c="c:\\foscamRecord";setCookie("Record"+IFs,c)}*/
        window.clearTimeout(hRecord[IFs]);
		this.obj[IFs].Record(t,"");
        
	}
    this.LocalAlarmRecord=function(t, channl)
    {
        this.obj[channl].Record(t, "alarm");
    }
	this.RfLive=function(){
		this.obj[IFs].GetLiveCfg();
	}
	this.GetDevIPandPort=function(t){
		this.obj[0].GetDevIPandPort(t);
	}
	this.VedioPlay = function(i,ip, cmd){
		if(cmd == 3){
			this.obj[i].Open("rtsp://"+ip+"/videoMain");
		}else if(cmd == 4){
			this.obj[i].Close();
		}
	}
	
	this.PbVedioPlay = function(ip, dir, file, cmd){
		if(cmd == 3){
			if(typeof file=='undefined') return false;
			this.obj[0].PlaybackOpen(dir+",rtsp://"+ip+"/"+file);
		}else if(cmd == 4){
			this.obj[0].PlaybackClose();
		}else if(cmd == 5){
			this.obj[0].PlaybackPause();
		}else if(cmd == 6){
			this.obj[0].PlaybackResume();
		}
		return true;
	}
	
	this.PBClose = function(){
		this.obj[0].PlaybackClose();
	}
	
	this.PbSeek = function(time){
		this.obj[0].PlaybackSeek(time*1);
	}
	
	this.PbPause = function(){
		this.obj[0].PlaybackPause();
	}
	
	this.PbResume = function(){
		this.obj[0].PlaybackResume();
	}
	
	this.OpenAudio = function(i){
		this.obj[i].OpenAudio();
	}
	
	this.CloseAudio = function(i){
		this.obj[i].CloseAudio();
	}
		
	this.MsDel = function(){
		return(this.obj[0].DelRect());
	}
		
	this.SetMotionArea = function(xml){
		return(this.obj[0].MotionAreas = xml)
	}
	
	this.GetMotionArea = function(xml){
		return(this.obj[0].MotionAreas)
	}
	
	this.SetAVArea = function(xml){
		return(this.obj[0].Areas = xml)
	}
	
	this.GetAVArea = function(){
		return(this.obj[0].Areas)
	}
	
	this.TalkCMD = function(i,cmd){
		if (cmd == 0){
			return(this.obj[i].OpenTalk())
		}else{
			return(this.obj[i].CloseTalk())
		}
	};

	this.ChangeWndSize = function (t, n) {
	    if (t == -1) {
	        this.obj[0].LeaveMotionSet();
	        return (this.obj[0].LeaveAreaSet());
	    } else if (t == 2) {
	        return (this.obj[0].EnterAreaSet(n, "rtsp://" + gVar.ip + "/videoMain", gVar.user, gVar.passwd, 800, 500));
	    } else if (t == 0) {
	        for (var i = 1; i < 9; i++) {
	            try {
	                this.obj[i].EnterConfigPage(0);
	            } catch (e) { }
	        }
	        return (this.obj[0].Live())
	    } else if (t == 1) {
	        for (var i = 1; i < 9; i++) {
	            try {
	                this.obj[i].EnterConfigPage(2);
	            } catch (e) { }
	        }
	        return (this.obj[0].Playback())
	    } else if (t == 3) {
	        return (this.obj[0].EnterMotionSet(n, "rtsp://" + gVar.ip + "/videoMain", gVar.user, gVar.passwd, 800, 500));
	    }
	    else if (t == 4) {
	        for (var i = 0; i < 9; i++) {
	            try {
	                this.obj[i].EnterConfigPage(1);
	            } catch (e) { }
	        }
	    }
	};
	
	this.SelectRecordPath = function(){
	    return(this.obj[0].ManualRecordPath);
	}
	
	
	this.test = function()
	{
		this.obj[this.index].test();
	}
	
	this.openMedia = function( mediaPath )
	{
		var isOpen = this.obj[this.index].OpenMedia(mediaPath);
	}
	
	this.closeMedia = function()
	{
		var isClose = this.obj[this.index].CloseMedia();
	}
	
	//play video
	this.play = function()
	{
		var isPlay = this.obj[this.index].Play();
	}
	
	this.pause = function()
	{
		var isPause = ocx.Pause();
	}
	
	this.stop = function()
	{
		var isStop = this.obj[this.index].Stop();
	}
	
	this.forwad = function(nSpeed)
	{
		var isOk = this.obj[this.index].Forward(nSpeed);
	}
	
	this.reverse = function(nSpeed)
	{
		var isOk = this.obj[this.index].Reverse(nSpeed);
	}
	
	this.perframe = function()
	{
		this.obj[this.index].PreFrame();
	}
	
	this.nextframe = function()
	{
		this.obj[this.index].NextFrame();
	}
	
	this.seek = function(pos, minVal, maxVal)
	{
		this.obj[this.index].Seek(pos, minVal, maxVal);
	}
	
	this.setWndNum = function(count)
	{
		if(count < 0 || count > 9){
			alert("please set value in [0-9]");
			return;
		}
		this.obj[this.index].SetWndStyle(0, count);
	}
	
	this.fullscreen = function()
	{
		this.obj[this.index].FullScreen();
	}
	 
	this.snap = function(path)
	{
		this.obj[this.index].Snap( path );
	}
}