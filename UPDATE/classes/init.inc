<?php

///////////////////////////////////////////////////
$con = mysqli_connect("localhost","root","", "uetcl_db");
if(!$con) echo 'No selecting'.mysqli_error($con);
//////////////////////////////////////////////////

session_start();
error_reporting(null);
include "classes/Db.php";
include "classes/Feedback.php";	
include "classes/Image.php";
			
include "classes/BeforeAndAfter.inc";
include "classes/AccessRights.inc";

include "classes/Dashboard.inc";
include "classes/Users.inc";
include "classes/Department.inc";
include "classes/Branches.inc";
include "classes/Vehicles.inc";
include "classes/RoutingTable.inc";
include "classes/CostCenters.inc";
include "classes/FuelCards.inc";
include "classes/Drivers.inc";
include "classes/VehicleRequest.inc";
include "classes/Designation.inc";
include "classes/ApprovalOrder.inc";
include "classes/Settings.inc";
include "classes/ApproveVehicleRequest.inc";
include "classes/VehicleReturn.inc";
include "classes/RefillStatements.inc";
include "classes/Products.inc";
include "classes/AuditTrail.inc";
include "classes/PoolFuelAccountability.inc";
//include "classes/TerritoryApprovalOrder";
include "classes/Territory.inc";
include "classes/AreaOffice.inc";
include "classes/TerritoryApprovalOrder.inc";
include "classes/ReturnTerritoryVehicle.inc";
include "classes/TerritoryVehicleAssignment.inc";
include "classes/PoolFuelRequest.inc";
include "classes/ApprovePoolFuelRequest.inc";
include "classes/HolderApprovalOrder.inc";
include "classes/PersonalToHolder.inc";
include "classes/PersonalToHolderFuelRequest.inc";
include "classes/ApprovePersonalToHolderFuelRequest.inc";
include "classes/ApprovePthfr.inc";
include "classes/Reports.inc";
include "classes/ReloadOrder.inc";
include "classes/District.inc";

include "classes/Services.inc";

function user_id(){
	$x = @$_SESSION['UEDCL_USER_ID'];
	return $x;
	return 4;
}

function return_url($url=""){
	$x = explode("/", $_SERVER["REQUEST_URI"]);
	return "http://".$_SERVER["HTTP_HOST"].'/'.$x[1].'/'.$url;		
}
function display_url($url=""){
	$x = explode("/", $_SERVER["REQUEST_URI"]);
	echo "http://".$_SERVER["HTTP_HOST"].'/'.$x[1].'/'.$url;		
}
function active($p=""){
	$portion = portion(1);
	$show = false;

	$className = ucwords(str_replace('-', ' ', $p));
	
	echo $className = ucwords(str_replace(' ','',$className));
	
	if(class_exists($className)){
		echo 'Yes';
		foreach($className::getLinks() as $link){
			extract($link);
			$a = new AccessRights();
			if($a->sectionAccess(user_id(), $link_page, $link_right)){
				$show = true;
			}else{
				$show = false;
			}
		}
	}else{
		echo "class does not exit";
		$show = true;
	}
	
	if($p == $portion && $show == true){
		echo ' class="active " ';
	}else if($p == $portion && $show == false){		
		echo ' class="active hiddenLink" ';
	}else if($p != $portion && $show == true){
		//echo $p.'===>'.$portion;
		//echo ' class=" hiddenLink" ';
	}else if($p != $portion && $show == false){		
		echo ' class=" hiddenLink" ';
	}
}

$portion = 0;
function portion($segment){
	$uri_array = explode('/',$_SERVER['REQUEST_URI']);
	$uri_count = count($uri_array);
	$returning_uri = array();

	for($i = 0;$i<$uri_count;$i++)
	{
		if(empty($uri_array[$i]) || $uri_array[$i] == "index.php")
			unset($uri_array[$i]);
		else
			array_push($returning_uri,$uri_array[$i]);
	}

	if($segment < count($returning_uri))
		return $returning_uri[$segment];
	else
		return false;
}

//$u = new Users();

$user_id = user_id();
$db = new Db();
$select = $db->select("SELECT user_forgot_password FROM sysuser WHERE user_id='$user_id'");
extract($select[0][0]);

if($user_forgot_password){		

	if( portion(2) == "logout"){

	}elseif(portion(2)!="changed-password"){
		header("Location:".return_url().'users/changed-password');
	}
}

include "classes/statics.inc";

?>