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

$time = time();
$user = user_id();

$user_id = user_id(); //$_SESSION[SNAME.'_USER_ID'];

$update = $db->update("sysuser", ["user_last_active"=>$time], ["user_id"=>$user_id]); 
