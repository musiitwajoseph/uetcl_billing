<?php 
error_reporting(1);
include "../classes/init.inc";
ini_set('display_errors',1);

// $system_name = "CENTENARY";
// $user_id = $_SESSION['CENTENARY_USER_ID'];
// if(empty($user_id)){
// 	Feedback::refresh(1, return_url()."users/logout");
// }


// $db = new Db();
// $sql = "SELECT * FROM sysuser WHERE user_id = '$user_id'";
// $select = $db->select($sql);
// extract($select[0]);

// if($user_id){
//     $time = time();
//     $update = $db->update("sysuser", ["user_last_active"=>$time], ["user_id"=>$_SESSION[$system_name.'_USER_ID'], "user_session"=>Feedback::ip_address()]); 
// }

$user_id = $_SESSION['CENTENARY_USER_ID'];
$attach_id = $_POST['attachmentID'];

$href = $_POST['href'];

$portion = explode('/',str_replace(return_url(), '', $href));
//print_r($portion);
$id = $portion[2];

$class = ($portion[0]=="")? 'Dashboard':$portion[0];
$method = ($portion[1]=="")? 'index':$portion[1];

$customer = $portion[2];
$year = $portion[3];
$month = $portion[4];

$class_name = convertToStudlyCaps($class);
$method_name = convertToCamelCase($method);

 // echo "$class_name and $method_name and $id";

if(class_exists($class_name)){
	//echo '<br/>'.$class_name.'<br/>';
	$class = new $class_name;
	
	if(is_callable([$class, $method_name])){
		if(strtolower($portion[0])=="services" || $portion[0] == "formulae-builder" || $portion[0] == "customers" || $portion[0] == "optic-fibre-customers" || $portion[0] == "users" || $portion[0] == "otherInvoices")
			$class->parameters($href);

		$class->$method_name();	
	}else{
		echo '<b>'.$method_name.'</b> METHOD DOES NOT EXIST in CLASS: '.$class;
	}	
}else{
	echo '<b>'.$class.'</b> CLASS DOES NOT EXIST';
}


function convertToStudlyCaps($string){
	return str_replace(' ', '', ucwords(str_replace('-', ' ', $string)));
}

function convertToCamelCase($string){
	return lcfirst(convertToStudlyCaps($string));
}