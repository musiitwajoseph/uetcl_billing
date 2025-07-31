<?php
include "../classes/Db.php";
include "../classes/Field_calculate.inc";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

$t = new BeforeAndAfter();

$user_id = "";
$portion = "advance-schedule";

if(1){
	$comment = $_POST['comment'];

	$is = $_POST['is'];
	$ip = $_POST['ip'];
	$io = $_POST['io'];
	$es = $_POST['es'];
	$ep = $_POST['ep'];
	$eo = $_POST['eo'];
	$mp_id = $_POST['mp_id'];
	$rea_id = $_POST['rea_id'];
	$year = $_POST['year'];
	$month = $_POST['month'];
	$customer_id = $_POST['customer_id'];
	$user_id = $_POST['user_id'];

	if(empty($comment)){
		FeedBack::error("Enter Comment");
	}else{
		$db = new Db();
		// $show = array("schedule", "advance-schedule", "view-invoice");
		
		// $showon = $_POST['showon'];
		// for($i=0; $i<count($showon); $i++){
		// 	$type = $show[$showon[$i]];
		// 	$select = $db->insert("readings_comment", ["rc_comment"=>$comment, "rc_date_added"=>time(), "rc_added_by"=>$user_id, "rc_customer_id"=>$customer_id, "rc_year"=>$year, "rc_month"=>$month, "rc_mp_id"=>$mp_id, "rc_type"=>$type, "rc_last"=>1]);
		// }

		$tou = $t->tou($customer_id, $month, $year);
		$peak = $tou[0];
		$shoulder = $tou[1];
		$off_peak = $tou[2];

		$select = $db->insert("readings_comment", ["rc_comment"=>$comment, "rc_date_added"=>time(), "rc_added_by"=>$user_id, "rc_customer_id"=>$customer_id, "rc_year"=>$year, "rc_month"=>$month, "rc_mp_id"=>$mp_id, "rc_type"=>$portion]);

		$x = array('is', 'ip', 'io', 'es', 'ep', 'eo');
		$h = array('tisu', 'tipu', 'tiou', 'tesu', 'tepu', 'teou');
		$v = array('tisa', 'tipa', 'tioa', 'tesa', 'tepa', 'teoa');
		$f = array($shoulder, $peak, $off_peak, $shoulder, $peak, $off_peak);

		$reg = $t->rgf("metering_point", $mp_id, "mp_id", "mp_region_id");
		$reg = ($reg == 10 || $reg == 8) ? 1: 1;

		for($i=0; $i<count($x); $i++){
			$y = $x[$i];
			if($$y != ""){
				$a = array();
				$a["rate_advance_".($i+1)] = str_replace(',', '', ($$y))*1000*$reg;
				$aaa[$v[$i]] = str_replace(',', '', ($$y))*1000*$reg;
				$aaa[$h[$i]] = str_replace(',', '', ($$y))*$f[$i]*$reg;

				$select = $db->update("r_rate", $a, ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rea_id]);
			}
		}

		//FeedBack::redirect(return_url()."/".portion(1)."/".$portion."/".portion(3)."/".portion(4)."/".portion(5)."#$rea_id");

			//imports ========================================================
			$aaa["tiu"] = $aaa["tisu"]+$aaa["tipu"]+$aaa["tiou"];
			$aaa["tia"] = $aaa["tisa"]+$aaa["tipa"]+$aaa["tioa"];

			//exports =======================================================
			$aaa["teu"] = $aaa["tesu"]+$aaa["tepu"]+$aaa["teou"];
			$aaa["tea"] = $aaa["tesa"]+$aaa["tepa"]+$aaa["teoa"];
			print_r($aaa);

			$db = new Db();
			if(1){ //imports as advance
				$select = $db->update("r_rate", $aaa, ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rea_id]);
			}else{ //exports as advance
				// $select = $db->update("r_rate",[
				// 	//$variable=>$value,
				// 	"rate_added_by"=>$user_id,
				// 	"rate_date_added"=>time(),
				// ], ["rate_cus_id"=>$customer_id, "rate_reading_id"=>$rrr]);
				// echo $db->error();
			}
















	}
}