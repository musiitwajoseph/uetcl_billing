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


if(1){

	$comment = $_POST['comment'];
	$tc = $part = $_POST['part'];
	$month = $_POST['month'];
	$year = $_POST['year'];
	$customer = $_POST['customer'];
	$count = $_POST['count'];
	$type = $_POST['type'];
	$parc_id = $_POST['parc_id'];

	$customername = $t->rgf("customer", $customer, "customer_id", "customer_short_name");
	
	if($tc != 3){		
		$db = new Db();
	    $db->query("UPDATE invoice_date SET id_invoice_date='$invoice_date', id_remarks='$remarks' WHERE id_year='$year' AND id_month='$month' and id_customer_id ='$customer'");
	}

	if($tc == 1){
		$t->assignInvoiceNumber($year, $month, $customer);

		$approveOne = $_POST["ApproveOne"];
		$approveTwo = $_POST["ApproveTwo"];
		$invoice_date = strtotime($_POST["invoice_date"]);
		$remarks = $_POST["remarks"];

		$ind['dl_year'] = $year;
		$ind['dl_month'] = $month;
		$ind['dl_date_added'] = time();
		$ind['dl_added_by'] = user_id();
		$ind['dl_selected1'] = $t->rgf("sysuser", user_id(), "user_id", "user_designation");
		$ind['dl_selected2'] = $approveOne;
		$ind['dl_selected3'] = $approveTwo;
		$ind['dl_customer'] = $customer;
	}else{

	}
	
		
	$ind["dl_app$tc"] = 1;
	
	$com = array();
	$com["arc_year"] = $year;
	$com["arc_month"] = $month;
	$com["arc_description"] = $comment;
	$com["arc_date_added"] = time();
	$com["arc_added_by"] = user_id();
	$com["arc_part"] = $part;
	$com["arc_customer"] = $customer;
	$com["arc_type"] = 'FIBRE';


	if($type == "reject"){
		$delete = $d->query("DELETE FROM done_levels WHERE dl_year = '$year' AND dl_month = '$month' AND dl_customer = '$customer'");
		$delete = $d->query("DELETE FROM all_readings_comments WHERE arc_year = '$year' AND arc_month = '$month' AND arc_customer = '$customer'");

		$message .= "<br/><br/>The invoices for <b>".$customername. " $year-".month_name($month)."</b> is ready for approval.<br/>";
			$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	
			$message .= $ms;

		    $subject = "Energy Invoice";
		    

	}else{

		$insert = $d->insert("all_readings_comments", $com);
		echo $d->error();
		//$link = return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5);
		
		if($tc==1){

			$approveOne = $_POST["ApproveOne"];
			$approveTwo = $_POST["ApproveTwo"];

			$insert = $d->insert("done_levels", $ind);
			echo $d->error();
			
			$next = $t->next($tc, $year, $month);
			$to = $next['email'];
			$message = "Hello ".$next['fname'].",";							
			
			$customer = $t->rgf("customer", $customer, "customer_id", "customer_short_name");

			$message .= "<br/><br/>The invoices for <b>".$customername. " $year-".month_name($month)."</b> is ready for approval.<br/>";
			$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	
			$message .= $ms;

		    $subject = "Energy Invoice";
		    //Feedback::sendmail($to,$subject,$message,$name);
		}else{

			$d = new Db();
			$insert = $d->update("done_levels", $ind, ["dl_id"=>$parc_id]);

			if($tc == 2){
				$next = $t->next($tc, $year, $month);
				$to = $next['email'];
				$message = "Hello ".$next['fname'].",";
				$message .= "<br/><br/>The invoice for <b>$customername $year-".month_name($month)."</b> is ready for approval.<br/>";
			}elseif($tc == 3){
				$next = $t->next($tc, $year, $month);
				$to = $next['email'];
				$message = "Hello ".$next['fname'].",";	
				$message .= "<br/><br/>The invoice for <b>$customername $year-".month_name($month)."</b> is Successfully approved.<br/>";						
			}
				
			
			$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";	

			$message .= $ms;
		    $subject = "Energy Invoice Approval";
		    //Feedback::sendmail($to,$subject,$message,$name);

		}
	}

	//FeedBack::redirect(return_url().portion(1)."/".portion(2)."/".portion(3)."/".portion(4)."/".portion(5));
}