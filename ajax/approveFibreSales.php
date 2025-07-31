<?php
ob_start();
session_start();
include "../classes/Db.php";
include "../classes/FeedBack.php";
include "../classes/Field_calculate.inc";
include "../classes/BeforeAndAfter.inc";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

$t = new BeforeAndAfter();

$db = new Db();
$d = new Db();

function user_id(){
	$x = @$_SESSION['UEDCL_USER_ID'];
	return $x;
	return 4;
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

//ini_set('display_errors', 1);


	   $ind = array();
		$comment = $_POST['comment'];
		$options = $_POST['options'];
		$selected_invoice_date = strtotime($_POST['selected_invoice_date']);
		//$cid = $options
		
		$part = $_POST['part'];
		$parc_id = $_POST['parc_id'];
		$tc = $part;
		$customer = $_POST['customer'];
		$gi = $_POST['gi'];
		//echo ">>>>>".$gi;

		$month = $_POST['month'];
	    $year = $_POST['year'];

		
		if($tc == 1){			
			$ApproveOne = $_POST["ApproveOne"];
			$ApproveTwo = $_POST["ApproveTwo"];	

	        						
			
		}
			
		$ind["not_app$tc"] = 1;
		
		$com = array();
		$com["arc_year"] = $year;
		$com["arc_month"] = $month;
		$com["arc_description"] = $comment;
		$com["arc_date_added"] = time();
		$com["arc_added_by"] = user_id();
		$com["arc_part"] = $part;
		$com["arc_customer"] = $customer;
		$com["arc_invoice_date"] = $selected_invoice_date;
		$com["arc_type"] = "FIBRE";

		//print_r($ind);

		$d = new Db();
		$insert = $d->insert("all_readings_comments", $com);
		echo $d->error();
		//$link = ''.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").''; //return_url().'optic-fibre-customers/approve-optic-fibre-invoices';
		
		if($tc==1){

			    $ApproveTwo = $_POST["ApproveTwo"];
			    $ApproveOne = $_POST["ApproveOne"];
			// echo '>>'.$ApproveOne;
			// echo '>>'.$ApproveTwo;
			$msg = "";
			//foreach($options as $cu){
				
				$ind['not_date_added'] = time();
				$ind['not_added_by'] = user_id();
				$ind['not_selected1'] = $t->rgf("sysuser", user_id(), "user_id", "user_designation");
				$ind['not_selected2'] = $ApproveOne;
				$ind['not_selected3'] = $ApproveTwo;

				$d = new Db();
			    $insert = $d->update("notification", $ind, ["not_id"=>$gi]);
				echo $d->error();

			// 	// $period = Feedback::date_s($start_date);
			// 	// $period .= " - ".Feedback::date_s($edl);

			// 	//$msg .= '<li><a href="'.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
				//echo ">>>>>>>>>>>>>>>>>>>>>>";
			//}
			
			// $next = $t->next($tc, $cid, $start_date);
			// $to = ($next['email'])?$next['email']:"josemusiitwa@gmail.com";
			// $message = "Hello ".$next['fname'].",";	

			// if(count($options) > 1){
			// 	$message .= "<br/><br/>The following invoices are ready for approval:<br/>";
			// }elseif(count($options) == 1){
			// 	$message .= "<br/><br/>The following invoice is ready for approval:<br/>";
			// }
			// $message .= '<ol>';
			// $message .= $msg;
			// $message .= '</ol>';
			// $message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	
			// $message .= $ms;

		 //    $subject = "Optic Fibre Invoice Approval";
		   
		   // Feedback::sendmail($to,$subject,$message,$name);
		}else{

			
		// 	$next = $t->next($tc, $cid, $start_date);
		// 	//print_r($next);
		// 	$to = ($next['email'])?$next['email']:"josemusiitwa@gmail.com";
		// 	$message = "Hello ".$next['fname'].",";

			if($tc == 2){
			// 	$msg = "";
				// foreach($options as $cu){
				// 	$z = $cu;
					
					//$ind['not_app'.$tc] = 1;

					$not_id;
					$d = new Db();
					$insert = $d->update("notification", $ind, ["not_id"=>$gi]);
					// $period = Feedback::date_s($start_date);
					// $period .= " - ".Feedback::date_s($edl);
					//$msg .= '<li><a href="'.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
				//}

		// 		if(count($options) > 1){
		// 			$message .= "<br/><br/>The following invoices are ready for approval:<br/>";
		// 		}elseif(count($options) == 1){
		// 			$message .= "<br/><br/>The following invoice is ready for approval:<br/>";
		// 		}
		// 		$message .= '<ol>';
		// 		$message .= $msg;
		// 		$message .= '</ol>';
					
		// 		$message .= $ms;
			    
			}elseif($tc == 3){
				// $msg = "";
				// foreach($options as $cu){
				// 	$z = $cu;
														
					$ind['not_app'.$tc] = 1;

					$not_id;
					$d = new Db();
					$insert = $d->update("notification", $ind, ["not_id"=>$gi]);
					// $period = Feedback::date_s($start_date);
					// $period .= " - ".Feedback::date_s($edl);
					//$msg .= '<li><a href="'.return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4).'">'.$this->rgf("of_customer", $cu, "ofc_id", "ofc_customer_name").'</a> &nbsp; ('.$period.')</li>';
				//}

		// 		if(count($options) > 1){
		// 			$message .= "<br/><br/>The following invoices are successfully approved:<br/>";
		// 		}elseif(count($options) == 1){
		// 			$message .= "<br/><br/>The following invoice is successfully approved:<br/>";
		// 		}
		// 		$message .= '<ol>';
		// 		$message .= $msg;
		// 		$message .= '</ol>';
					
		// 		$message .= $ms;					
			}
				
			
		// 	$message .= "<br/>You can use this link: <a href='$link'>$link</a>";	
		// 	$subject = "Optic Fibre Invoice Approval";
		//     //Feedback::sendmail($to,$subject,$message,$name);
			
		}
