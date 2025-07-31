<?php

///////////////////////////////////////////////////

define('EMAIL_SMTP_DEBUG', '2');                //2
define('EMAIL_SERVER', 'uetcl-com.mail.protection.outlook.com');    //HOST
define('EMAIL_SMTP_AUTH', FALSE);       
define('EMAIL_PORT', 25);
define('EMAIL_USERNAME', 'info@uetcl.com');
define('EMAIL_PASSWORD', '1616DD????');
define('EMAIL_SMTP_SECURE', 'tls');
define('EMAIL_SET_FROM', 'info@uetcl.com');

session_start();
//error_reporting(null);
(!defined('ABSPATH')) ? define('ABSPATH', dirname(__FILE__) . '/') : "";


define('AD_DOMAIN', '@uetcl.com');
define('AD_DNS_NAME', 'DC-vm2');
define('AD_DN', 'DC=UETCL,DC=COM');

session_start();
date_default_timezone_set("Africa/Kampala");

include ABSPATH."Db.php";
include ABSPATH."DbSunServer.php";
include ABSPATH."Feedback.php"; 
include ABSPATH."Image.php";  
//echo ABSPATH."Feedback.php";

spl_autoload_register(function ($class_name){
    $root = str_replace('\\','/', ABSPATH);
    $file = str_replace('\\','/', $root).''.$class_name.'.inc';
    if(is_readable($file)){
        include $file;
    }
    
});

function user_id(){
	$x = @$_SESSION['UEDCL_USER_ID'];
	return $x;
	return 4;
}

/*
function return_url($url=""){
	$x = explode("/", $_SERVER["REQUEST_URI"]);
	return "https://".$_SERVER["HTTP_HOST"].'/'.$url;		
}
function display_url($url=""){
	$x = explode("/", $_SERVER["REQUEST_URI"]);
	echo "https://".$_SERVER["HTTP_HOST"].'/'.$url;		
}
*/


function return_url($url=""){
	$x = explode("/", $_SERVER["REQUEST_URI"]);
	return "https://".$_SERVER["HTTP_HOST"].'/'.$x[1].'/'.$url;		
}
function display_url($url=""){
	$x = explode("/", $_SERVER["REQUEST_URI"]);
	echo "https://".$_SERVER["HTTP_HOST"].'/'.$x[1].'/'.$url;		
}

function activeClass($p=""){
	$portion = portion(2);
	if($portion == $p){
		echo ' class="active" ';
	}
}

function active($p=""){
	$portion = portion(1);
	$show = false;

	$className = ucwords(str_replace('-', ' ', $p));
	
	$className = ucwords(str_replace(' ','',$className));
	
	if(class_exists($className)){
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

function portionOLD($segment){
	$segment -= 1;
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
		//header("Location:".return_url().'users/changed-password');
	}
}

function system_mode($mode=0){

	$production = array(
		'p', 
		'pro', 
		'production'
	);

	if(in_array(strtolower($mode), $production)){
		error_reporting(null);
		ini_set('log_errors', 0);
	}else{
		ini_set('display_errors', 1);
		ini_set('display_startup_errors', 1);
		error_reporting(E_ALL);
	}
}

function error($error_array) {
    echo '<div id="error">';
    echo "Please review the following fields:<br />";
    echo '<div style="padding:2px 20px;">';
    foreach($error_array as $error) {
        echo " - " . $error . "<br />";
    }
    echo '</div>';
    echo "</div>";
}
function refresh($seconds = 3, $link = ""){
    if($link == ""){
        echo '<meta http-equiv="refresh" content="'.$seconds.'"/>';
    }else{
        echo '<meta http-equiv="refresh" content="'.$seconds.';'.$link.'"/>';
    }
}
function returnFieldFromCustomer($id, $toReturn){
    global $con;
    $select = mysqli_query($con, "select $toReturn from customers where cus_id = $id");
    $row = mysqli_fetch_assoc($select);
    return $row[$toReturn];
}

function returnFieldFromService($id, $toReturn){
    global $con;
    $select = mysqli_query($con, "select $toReturn from service where se_id = $id");
    $row = mysqli_fetch_assoc($select);
    return $row[$toReturn];
}

function month_name($num){
    $monthNum  = $num;
    $dateObj   = DateTime::createFromFormat('!m', $monthNum);
    return $monthName = $dateObj->format('F'); 
}

function month_name_short($num){
    $monthNum  = $num;
    $dateObj   = DateTime::createFromFormat('!m', $monthNum);
    return $monthName = $dateObj->format('M'); 
}

function rate_calculate2($customer_id, $rea_date="", $mp_id=""){   
    $to_return = array(); 
    if($mp_id == '5315CUSTOM'){
        $mp_id = 5315;
        $mp_id_custom = '5315CUSTOM';
    }

     $cal = rate_calculate($customer_id, $rea_date, $mp_id);
    if($customer_id == 1017){ //BECS        
        $g1 = ($cal[0]);// - $cal[4]);
        $g2 = ($cal[1]);// - $cal[5]);
        $g3 = ($cal[2]);// - $cal[6]);

        $g4 = $g1 + $g2 + $g3;

        $tc1 += $g1;
        $tc2 += $g2;
        $tc3 += $g3;
        $tc4 += $g4;                     
    }elseif($customer_id == 2021 && $mp_id == '9383'){ //KIL => Kyambura shp ziba
        
        $g1 = ($cal[0] - $cal[4]);
        $g2 = ($cal[1] - $cal[5]);
        $g3 = ($cal[2] - $cal[6]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 += $g1;
        $tc2 += $g2;
        $tc3 += $g3;
        $tc4 += $g4;

    }elseif($customer_id == 2021 && $mp_id == '5302'){ //KIL => Kyambura shp ziba
        
        $g1 = ($cal[0] - $cal[4]);
        $g2 = ($cal[1] - $cal[5]);
        $g3 = ($cal[2] - $cal[6]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 += $g1;
        $tc2 += $g2;
        $tc3 += $g3;
        $tc4 += $g4;

    }elseif($customer_id == 2021 && $mp_id == 5310){ //KIL
        
        $cal_2 = rate_calculate($customer_id, ($rea_date), 5303); //Kikorongo
        $cal_3 = rate_calculate(2017, ($rea_date), 4009); //SNEL -> Mpondwe

        $g1 = ($cal[4] - $cal[0] - $cal_2[4] - $cal_3[0]);
        $g2 = ($cal[5] - $cal[1] - $cal_2[5] - $cal_3[1]);
        $g3 = ($cal[6] - $cal[2] - $cal_2[6] - $cal_3[2]);
        $g4 =$g1 + $g2 + $g3;

        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }else{
        $tc1 = $cal[0];
        $tc2 = $cal[1];
        $tc3 = $cal[2];
        $tc4 = $cal[3];
    }

    $tc5 = $cal[4];
    $tc6 = $cal[5];
    $tc7 = $cal[6];
    $tc8 = $cal[7];
    
    return array($tc1, $tc2, $tc3, $tc4, $tc5, $tc6, $tc7, $tc8);    
}

function rate_calculate2_cn($customer_id, $rea_date="", $mp_id=""){   
    $to_return = array(); 
    if($mp_id == '5315CUSTOM'){
        $mp_id = 5315;
        $mp_id_custom = '5315CUSTOM';
    }

     $cal = rate_calculate_cn($customer_id, $rea_date, $mp_id);
    if($customer_id == 1017){ //BECS        
        $g1 = ($cal[0] - $cal[4]);
        $g2 = ($cal[1] - $cal[5]);
        $g3 = ($cal[2] - $cal[6]);

        $g4 = $g1 + $g2 + $g3;

        $tc1 += $g1;
        $tc2 += $g2;
        $tc3 += $g3;
        $tc4 += $g4;                     
    }elseif($customer_id == 2020 && $mp_id == 5331){ //UEDCL => ecopower
        
        $cal_2 = rate_calculate_cn($customer_id, ($rea_date), 5278); //muhanga rugyeyo
        $cal_3 = rate_calculate_cn($customer_id, ($rea_date), 5274); //nyakagyeme

        $g1 = ($cal[0] - $cal[4] - $cal_2[4] - $cal_3[4]);
        $g2 = ($cal[1] - $cal[5] - $cal_2[5] - $cal_3[5]);
        $g3 = ($cal[2] - $cal[6] - $cal_2[6] - $cal_3[6]);

        $g4 = $g1 + $g2 + $g3;

        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }elseif($customer_id == 2020 && $mp_id == 5297){ //UEDCL => nkuusi
        
        $cal_2 = rate_calculate_cn($customer_id, ($rea_date), 5284); //munteme
        $cal_3 = rate_calculate_cn($customer_id, ($rea_date), 5275); //rugombe
        $cal_4 = rate_calculate_cn($customer_id, ($rea_date), 5276); //kakumiro

        $g1 = ($cal[0] - $cal[4] - $cal_2[4] - $cal_3[4] - $cal_4[4]);
        $g2 = ($cal[1] - $cal[5] - $cal_2[5] - $cal_3[5] - $cal_4[5]);
        $g3 = ($cal[2] - $cal[6] - $cal_2[6] - $cal_3[6] - $cal_4[6]);

        $g4 = $g1 + $g2 + $g3;

        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }elseif($customer_id == 2020 && $mp_id == 5298){ //UEDCL => waki hydro max
        
        $cal_2 = rate_calculate_cn($customer_id, ($rea_date), 5291); //burindi exports

        $g1 = ($cal[4] - $cal[0] - $cal_2[4] );
        $g2 = ($cal[5] - $cal[1] - $cal_2[5] );
        $g3 = ($cal[6] - $cal[2] - $cal_2[6] );

        $g4 = $g1 + $g2 + $g3;

        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }elseif($customer_id == 2020 && $mp_id == 5326){ //UEDCL => kamwenge
        
        $tc1 += $cal[0];
        $tc2 += $cal[1];
        $tc3 += $cal[2];
        $tc4 += $cal[3];

    }elseif($customer_id == 2020 && $mp_id == '5327'){ //UEDCL => siti 1 5327

        $cal_2 = rate_calculate_cn(2020, ($rea_date), 5267); //kweene exports   

        $g1 = ($cal[0] - $cal[4]);
        $g2 = ($cal[1] - $cal[5]);
        $g3 = ($cal[2] - $cal[6]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }elseif($customer_id == 2020 && $mp_id == '5328'){ //UEDCL => siti 2 5328
        $cal_2 = rate_calculate_cn(2020, ($rea_date), 5267); //kweene exports   

        $g1 = ($cal[0] - $cal[4] - $cal_2[4]);
        $g2 = ($cal[1] - $cal[5] - $cal_2[5]);
        $g3 = ($cal[2] - $cal[6] - $cal_2[6]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }elseif($customer_id == 2022 && $mp_id_custom == '5315CUSTOM'){ //KRECs => kiseruka 5315
        $g1 = ($cal[4]);
        $g2 = ($cal[5]);
        $g3 = ($cal[6]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }elseif($customer_id == 2022 && $mp_id == 5315){ //KRECs => kiseruka 5315
        //echo "here<br/>";
        $g1 = ($cal[4] - $cal[0]);
        $g2 = ($cal[5] - $cal[1]);
        $g3 = ($cal[6] - $cal[2]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }elseif($customer_id == 2021 && $mp_id == '5302'){ //KIL => Kyambura shp ziba

        $g1 = ($cal[0] - $cal[4]);
        $g2 = ($cal[1] - $cal[5]);
        $g3 = ($cal[2] - $cal[6]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 += $g1;
        $tc2 += $g2;
        $tc3 += $g3;
        $tc4 += $g4;

    }elseif($customer_id == 2021 && $mp_id == '9383'){ //KIL => Kyambura shp ziba

        $g1 = ($cal[0] - $cal[4]);
        $g2 = ($cal[1] - $cal[5]);
        $g3 = ($cal[2] - $cal[6]);

        $g4 = $g1 + $g2 + $g3;
        $tc1 += $g1;
        $tc2 += $g2;
        $tc3 += $g3;
        $tc4 += $g4;

    }elseif($customer_id == 2021 && $mp_id == 5310){ //KIL
        
        $cal_2 = rate_calculate_cn($customer_id, ($rea_date), 5303); //Kikorongo
        $cal_3 = rate_calculate_cn(2017, ($rea_date), 4009); //SNEL -> Mpondwe

        $g1 = ($cal[4] - $cal[0] - $cal_2[4] - $cal_3[0]);
        $g2 = ($cal[5] - $cal[1] - $cal_2[5] - $cal_3[1]);
        $g3 = ($cal[6] - $cal[2] - $cal_2[6] - $cal_3[2]);
        $g4 =$g1 + $g2 + $g3;

        $tc1 = $g1;
        $tc2 = $g2;
        $tc3 = $g3;
        $tc4 = $g4;

    }else{
        $tc1 = $cal[0];
        $tc2 = $cal[1];
        $tc3 = $cal[2];
        $tc4 = $cal[3];
    }

    $tc5 = $cal[4];
    $tc6 = $cal[5];
    $tc7 = $cal[6];
    $tc8 = $cal[7];
    
    return array($tc1, $tc2, $tc3, $tc4, $tc5, $tc6, $tc7, $tc8);    
}


function rate_calculate($cus_id, $date="", $metering_point=""){
    
    $db = new Db();
    global $con;
    if(empty($date)){
        //sdafaf
        $sel = $db->select("SELECT TOP 1 rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC");
        extract($sel[0][0]);
    }else{
        $year = date('Y', @$date);
        $month = date('m', @$date);

        $start_date = strtotime($year.'-'.$month.'-01 12:00:00 am');
        $end_date = $year.'-'.$month.'-'.date('t', $start_date).' 11:59:59 pm';
        $end_date = strtotime($end_date);

        $db = new Db();
        $sql = "SELECT rea_id as x, rea_date as y FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point'  AND rea_date >= '$start_date' AND rea_date <= '$end_date'";
        $sel = $db->select($sql);
        //year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND
        if($db->num_rows()){
        foreach($sel[0] as $row){
            extract($row);
            if($year == date('Y', $y) && $month == date('m', $y) ){
                $rea_id = $rea_idlast = $x;
                $rea_date = $y;
                //echo 'reached';
            }
        }
        }
    }
    
    $sql = "SELECT TOP 1 rate_import_wh_1 as rat1, rate_import_wh_2 as rat2, rate_import_wh_3 as rat3,rate_export_wh_4 as rat4,rate_export_wh_5 as rat5,rate_export_wh_6 as rat6, rate_cus_id, rate_advance_1 as ais, rate_advance_2 as aip, rate_advance_3 as aio, rate_advance_4 as aes, rate_advance_5 as aep, rate_advance_6 as aeo, rate_swap, rate_label as label, rate_sfi as sfi, rate_sfe as sfe, rate_wheeling_charge as wheeling_charge, rate_wc_sfe as wc_sfe, rate_wc_sfi as wc_sfi, rate_wc_lfe as wc_lfe, rate_wc_lfi as wc_lfi, rate_cum_imports as cum_imports1, rate_cum_exports as cum_exports1 FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'";
    $sel = $db->select($sql);
    extract($sel[0][0]);
    //print_r($select);

    //$rea_date = date('m', ($rea_date));
    $db = new Db();
    $sql = "SELECT TOP 1 rea_id FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point' AND rea_date < $rea_date ORDER BY rea_date DESC";
    $sel = $db->select($sql);  
    extract($sel[0][0]);
    
    //echo $rea_date;

    if($db->num_rows()){      
        //selecting rates
       $sql = "SELECT TOP 1 rate_import_wh_1 as rate1, rate_import_wh_2 as rate2, rate_import_wh_3 as rate3,rate_export_wh_4 as rate4, rate_export_wh_5 as rate5,rate_export_wh_6 as rate6, rate_cus_id, rate_cum_exports, rate_cum_imports FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id'";
        $sel = $db->select($sql);
        extract($sel[0][0]); 
    }


    $cum_imports1_ = $cum_imports1-$rate_cum_imports;
    $cum_exports1_ = $cum_exports1-$rate_cum_exports;

    if(1){
        $r1 = ceil(@$rat1 - @$rate1);
        $r2 = ceil(@$rat2 - @$rate2);
        $r3 = ceil(@$rat3 - @$rate3);
        $r4 = ceil(@$rat4 - @$rate4);
        $r5 = ceil(@$rat5 - @$rate5);
        $r6 = ceil(@$rat6 - @$rate6);
    }else{
        $r1 = ceil(@$rat4 - @$rate4);
        $r2 = ceil(@$rat5 - @$rate5);
        $r3 = ceil(@$rat6 - @$rate6);
        $r4 = ceil(@$rat1 - @$rate1);
        $r5 = ceil(@$rat2 - @$rate2);
        $r6 = ceil(@$rat3 - @$rate3);
    }
    if($ais !="" || $aip != "" || $aio != "")
        $b=1;
    else
        $b=0;
    
    if(($ais != "")){ 
        $r1 = $ais;
    }
    if(($aip != "")){ 
        $r2 = $aip;
    }
    if(($aio != "")){ 
        $r3 = $aio;        
    }
    if(($aes != "")){ 
        $r4 = $aes;
    }
    if(($aep != "")){ 
        $r5 = $aep;
    }
    if(($aeo != "")){ 
        $r6 = $aeo;
    }
    
    if($rate_swap){
        $j1 = $r4;
        $j2 = $r5;
        $j3 = $r6;
        $j4 = $r1;
        $j5 = $r2;
        $j6 = $r3;
        
        $r1 = $j1; $r2=$j2; $r3=$j3; $r4=$j4; $r5=$j5; $r6=$j6;
    }
    
    $x = ($r1-$r4);
    $y = ($r2-$r5);
    $z = ($r3-$r6);

    //                  0   1   2        3        4    5   6       7       8  9  10      11
    $to_return = array($r1,$r2,$r3,($r1+$r2+$r3),$r4,$r5,$r6,($r4+$r5+$r6),$x,$y,$z, ($x+$y+$z), 
        $rate1, // 12
        $rate2, // 13
        $rate3, // 14
        $rate4, // 15
        $rate5, // 16
        $rate6,  // 17
        $rate_swap,//18
        array(//19
            $rat1,
            $rat2,
            $rat3,
            $rat4,
            $rat5,
            $rat6,
        ),
        array(//20
            $rate1,
            $rate2,
            $rate3,
            $rate4,
            $rate5,
            $rate6,
        ),
        $label,//21
        $sfi,//22
        $sfe,//23
        $wheeling_charge,//24
        $b,//25
        $rea_idlast,//26
        $wc_sfi,//27
        $wc_sfe,//28
        $wc_lfi,//29
        $wc_lfe,//30
        $cum_imports1,//31
        $cum_exports1,//32
        $cum_imports1_,//33
        $cum_exports1_,//34
    );
    
    return $to_return;

    
}


function rate_calculate_cn($cus_id, $date="", $metering_point=""){
        
    $db = new Db();
    global $con;
    if(empty($date)){
        //sdafaf
        $sel = $db->select("SELECT TOP 1 n_rea_id as rea_id, n_rea_date as rea_date FROM r_reading_cn WHERE n_rea_cus_id = $cus_id AND n_rea_mp_id = '$metering_point' ORDER BY n_rea_date DESC");
        extract($sel[0][0]);
    }else{
        $year = date('Y', @$date);
        $month = date('m', @$date);

        $start_date = strtotime($year.'-'.$month.'-01 12:00:00 am');
        $end_date = $year.'-'.$month.'-'.date('t', $start_date).' 11:59:59 pm';
        $end_date = strtotime($end_date);

        $db = new Db();
        $sql = "SELECT n_rea_id as x, n_rea_date as y FROM r_reading_cn WHERE n_rea_cus_id = $cus_id  AND n_rea_mp_id = '$metering_point'  AND n_rea_date >= '$start_date' AND n_rea_date <= '$end_date'";
        $sel = $db->select($sql);
        //year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND
        if($db->num_rows()){
        foreach($sel[0] as $row){
            extract($row);
            if($year == date('Y', $y) && $month == date('m', $y) ){
                $rea_id = $rea_idlast = $x;
                $rea_date = $y;
                //echo 'reached';
            }
        }
        }
    }

    
    $sel = $db->select("SELECT TOP 1 n_rate_import_wh_1 as rat1, n_rate_import_wh_2 as rat2, n_rate_import_wh_3 as rat3,n_rate_export_wh_4 as rat4,n_rate_export_wh_5 as rat5,n_rate_export_wh_6 as rat6, n_rate_cus_id, n_rate_advance_1 as ais, n_rate_advance_2 as aip, n_rate_advance_3 as aio, n_rate_advance_4 as aes, n_rate_advance_5 as aep, n_rate_advance_6 as aeo, n_rate_swap, n_rate_label as label, n_rate_sfi as sfi, n_rate_sfe as sfe, n_rate_wheeling_charge as wheeling_charge, n_rate_wc_sfe as wc_sfe, n_rate_wc_sfi as wc_sfi, n_rate_wc_lfe as wc_lfe, n_rate_wc_lfi as wc_lfi FROM r_rate_cn WHERE n_rate_reading_id = '$rea_id' AND n_rate_cus_id = '$cus_id'");
    extract($sel[0][0]);

    //print_r($select);

    //$rea_date = date('m', ($rea_date));
    $db = new Db();
    $sel = $db->select("SELECT TOP 1 n_rea_id as rea_id FROM r_reading_cn_previous WHERE n_rea_cus_id = $cus_id  AND n_rea_mp_id = '$metering_point' AND n_rea_date = $rea_date ORDER BY n_rea_date DESC");  
    extract($sel[0][0]);
    
    //echo $rea_date;

    if($db->num_rows()){      
        //selecting rates
        $sel = $db->select("SELECT TOP 1 n_rate_import_wh_1 as rate1, n_rate_import_wh_2 as rate2, n_rate_import_wh_3 as rate3,n_rate_export_wh_4 as rate4, n_rate_export_wh_5 as rate5,n_rate_export_wh_6 as rate6, n_rate_cus_id FROM r_rate_cn_previous WHERE n_rate_reading_id = '$rea_id' AND n_rate_cus_id = '$cus_id'");
        extract($sel[0][0]); 
    }

    if(1){
        $r1 = ceil(@$rat1 - @$rate1);
        $r2 = ceil(@$rat2 - @$rate2);
        $r3 = ceil(@$rat3 - @$rate3);
        $r4 = ceil(@$rat4 - @$rate4);
        $r5 = ceil(@$rat5 - @$rate5);
        $r6 = ceil(@$rat6 - @$rate6);
    }else{
        $r1 = ceil(@$rat4 - @$rate4);
        $r2 = ceil(@$rat5 - @$rate5);
        $r3 = ceil(@$rat6 - @$rate6);
        $r4 = ceil(@$rat1 - @$rate1);
        $r5 = ceil(@$rat2 - @$rate2);
        $r6 = ceil(@$rat3 - @$rate3);
    }
    if($ais !="" || $aip != "" || $aio != "")
        $b=1;
    else
        $b=0;
    
    if(($ais != "")){ 
        $r1 = $ais;
    }
    if(($aip != "")){ 
        $r2 = $aip;
    }
    if(($aio != "")){ 
        $r3 = $aio;        
    }
    if(($aes != "")){ 
        $r4 = $aes;
    }
    if(($aep != "")){ 
        $r5 = $aep;
    }
    if(($aeo != "")){ 
        $r6 = $aeo;
    }
    
    if($rate_swap){
        $j1 = $r4;
        $j2 = $r5;
        $j3 = $r6;
        $j4 = $r1;
        $j5 = $r2;
        $j6 = $r3;
        
        $r1 = $j1; $r2=$j2; $r3=$j3; $r4=$j4; $r5=$j5; $r6=$j6;
    }
    
    $x = ($r1-$r4);
    $y = ($r2-$r5);
    $z = ($r3-$r6);

    //                  0   1   2        3        4    5   6       7       8  9  10      11
    $to_return = array($r1,$r2,$r3,($r1+$r2+$r3),$r4,$r5,$r6,($r4+$r5+$r6),$x,$y,$z, ($x+$y+$z), 
        $rate1, // 12
        $rate2, // 13
        $rate3, // 14
        $rate4, // 15
        $rate5, // 16
        $rate6,  // 17
        $rate_swap,//18
        array(//19
            $rat1,
            $rat2,
            $rat3,
            $rat4,
            $rat5,
            $rat6,
        ),
        array(//20
            $rate1,
            $rate2,
            $rate3,
            $rate4,
            $rate5,
            $rate6,
        ),
        $label,//21
        $sfi,//22
        $sfe,//23
        $wheeling_charge,//24
        $b,//25
        $rea_idlast,//26
        $wc_sfi,//27
        $wc_sfe,//28
        $wc_lfi,//29
        $wc_lfe,//30
    );
    
    return $to_return;
    
}

function number_to_words($num){
        $num    = ( string ) ( ( int ) $num );
       
        if( ( int ) ( $num ) && ctype_digit( $num ) )
        {
            $words  = array( );
           
            $num    = str_replace( array( ',' , ' ' ) , '' , trim( $num ) );
           
            $list1  = array('','one','two','three','four','five','six','seven',
                'eight','nine','ten','eleven','twelve','thirteen','fourteen',
                'fifteen','sixteen','seventeen','eighteen','nineteen');
           
            $list2  = array('','ten','twenty','thirty','forty','fifty','sixty',
                'seventy','eighty','ninety','hundred');
           
            $list3  = array('','thousand','million','billion','trillion',
                'quadrillion','quintillion','sextillion','septillion',
                'octillion','nonillion','decillion','undecillion',
                'duodecillion','tredecillion','quattuordecillion',
                'quindecillion','sexdecillion','septendecillion',
                'octodecillion','novemdecillion','vigintillion');
           
            $num_length = strlen( $num );
            $levels = ( int ) ( ( $num_length + 2 ) / 3 );
            $max_length = $levels * 3;
            $num    = substr( '00'.$num , -$max_length );
            $num_levels = str_split( $num , 3 );
           
            foreach( $num_levels as $num_part )
            {
                $levels--;
                $hundreds   = ( int ) ( $num_part / 100 );
                $hundreds   = ( $hundreds ? ' ' . $list1[$hundreds] . ' Hundred' . ( $hundreds == 1 ? '' : '' ) . ' ' : '' );
                $tens       = ( int ) ( $num_part % 100 );
                $singles    = '';
               
                if( $tens < 20 )
                {
                    $tens   = ( $tens ? ' ' . $list1[$tens] . ' ' : '' );
                }
                else
                {
                    $tens   = ( int ) ( $tens / 10 );
                    $tens   = ' ' . $list2[$tens] . ' ';
                    $singles    = ( int ) ( $num_part % 10 );
                    $singles    = ' ' . $list1[$singles] . ' ';
                }
                $words[]    = $hundreds . $tens . $singles . ( ( $levels && ( int ) ( $num_part ) ) ? ' ' . $list3[$levels] . ' ' : '' );
            }
           
            $commas = count( $words );
           
            if( $commas > 1 )
            {
                $commas = $commas - 1;
            }
           
            $words  = implode( ', ' , $words );
           
            //Some Finishing Touch
            //Replacing multiples of spaces with one space
            $words  = trim( str_replace( ' ,' , ',' , trim_all( ucwords( $words ) ) ) , ', ' );
            if( $commas )
            {
                $words  = str_replace_last( ',' , ' ' , $words );
            }
           
            return $words;
        }
        else if( ! ( ( int ) $num ) )
        {
            return 'Zero';
        }
        return '';
    }
system_mode('p');


//date_default_timezone_set("Africa/Kampala");


include "classes/statics.inc";

?>