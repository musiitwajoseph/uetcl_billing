<?php
session_start();
error_reporting(null);
include("../classes/Db.php");
include("../classes/BeforeAndAfter.inc");	

function user_id(){
	$x = @$_SESSION['UEDCL_USER_ID'];
	return $x;
	return 4;
}

$t = new BeforeAndAfter();			
$minutes = 30;
$time = time();

$user = user_id();//$_SESSION[SNAME.'_USER_ID'];

$db = new Db();
$sql = "SELECT user_last_active FROM sysuser WHERE user_id = '$user'";
$select = $db->select($sql);
if($db->num_rows())
extract($select[0][0]);

$xx = array();
// $user_last_active = time()-rand(23, 45);
if(time() - $user_last_active > $minutes*60 && $user){
	$xx['status'] = 'expired';
}else{
	$xx['status'] = 'active';
}

$v = $minutes*60 - (time()-$user_last_active);
if($v < 0) $v = 0;

if($v <= 0){
	$min = str_pad((int)($v/60), 2, '0', STR_PAD_LEFT);
	$sec = str_pad((int)($v%60), 2, '0', STR_PAD_LEFT);
	$v = $min.':'.$sec;
$xx['remaining'] = '<div class="alert alert-danger" style="box-shadow:0 0 100px #000; border-radius:5px; text-align:center; position:fixed; width:300px; top:30%; left:45%;"><b>Your session has expired, <br/>You are required to login again</b><br/> <br/><button onclick="location.reload()" type="button"><i class="fa fa-fw fa-lock" title="Login"></i> Login</button><div>';
}elseif($v <= 60){
	$min = str_pad((int)($v/60), 2, '0', STR_PAD_LEFT);
	$sec = str_pad((int)($v%60), 2, '0', STR_PAD_LEFT);
	$v = $min.':'.$sec;
$xx['remaining'] = '<div class="alert alert-danger" style="box-shadow:0 0 100px #000; border-radius:5px; text-align:center; position:fixed; width:300px; top:30%; left:45%;"><b>Your session is about to expire</b><br/><span>Remaining: <b>'.$v.'s<b></span> <br/><button id="updateSession" type="button"><i class="fa fa-fw fa-refresh" title="Update Session"></i> Update Session</button><div>';
}else
 	$xx['remaining'] = '';

$min = str_pad((int)($v/60), 2, '0', STR_PAD_LEFT);
$sec = str_pad((int)($v%60), 2, '0', STR_PAD_LEFT);
$v = $min.':'.$sec;

$xx['actual'] = 'Session Timer: '.$v;
		   
echo json_encode($xx);