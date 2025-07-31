<?php

include "../classes/init.inc";

$t = new BeforeAndAfter();

ini_set('display_errors', 1);

$type = $_POST['formType'];
$choice = $_POST['choice'];
$comment = $_POST['comment'];
$reminder = $_POST['reminder'];
$count = $_POST['count'];
$id = $_POST['id'];
$requestor = $_POST['requestor'];
$reference = $_POST['reference'];
$counter_field = $_POST['counter_field'];
$table = $_POST['table'];
$status = $_POST['status'];
$id_field = $_POST['id_field'];

$signature = $_POST['signature'];

$user = user_id();

if($choice == "Approve"){

	$n = new Db();
	$item = array();
	$item['oi_counter'] = $count+1;

	$item2 = array();
	$item2[$id_field] = $id;

	$n->update($table, $item, $item2);


	$a = array();

	$db = new Db();
	$sql = "SELECT app_role_id as approved FROM master_approval_orders WHERE app_type = '$type' AND app_count = '$count' ORDER BY app_count DESC";
	$select = $db->select($sql);

	if($db->num_rows())
		extract($select[0][0]);

	$db = new Db();
	$next_count = $count+1;
	$sql = "SELECT app_role_id as forwarded FROM master_approval_orders WHERE app_type = '$type' AND app_count = '$next_count' ORDER BY app_count DESC";
	$select = $db->select($sql);

	if($db->num_rows())
		extract($select[0][0]);

	$a['ma_app_role_'.$count] = $approved;
	$a['ma_match_id'] = $id;
	$a['ma_type'] = $type;
	$a['ma_app_'.$count] = 1;
	$a['ma_user_id_'.$count] = $user;
	$a['ma_date_added_'.$count] = time();
	$a['ma_comment_'.$count] = $comment;

	$db = new Db();
	//checking if it already exists
	$sql = "SELECT ma_id FROM master_approvals WHERE ma_match_id = '$id' AND ma_type = '$type'";
	$select = $db->select($sql);
	if($db->num_rows()){
		extract($select[0][0]);
		$db = new Db();
		$b = array();
		$update = $db->update("master_approvals", $a , ["ma_id"=>$ma_id]);
	}else{
        $db = new Db();
		$db->insert("master_approvals", $a);
	}


	$db = new Db();
	$sql = "SELECT TOP 1 app_count FROM master_approval_orders WHERE app_type = '$type'  AND  app_role_id IS NOT NULL AND app_role_id != 0 ORDER BY app_count DESC";
	$select = $db->select($sql);
	extract($select[0][0]);
    
	if($app_count == $count){
		$n = new Db();
		$item = array();
		$item[$status] = 10;

		$item2 = array();
		$item2[$id_field] = $id; 

		$n->update($table, $item, $item2);

		$users_emails = array();
		$users_emails[] = $t->rgf("sysuser", $requestor, "user_id", "user_email");
		$subject = $type1.' '.$reference.' Completely Approved';
		$message = "";
		$message .= "Dear ".$t->full_name($requestor).', <br/><br/>';
		$message .= "Your ".$type1." with reference No.: <b>".$reference."</b> has been successfully approved ";
		
		// FeedBack::sendmailz($users_emails,$subject,$message,$hod_name);
		$xx['details'] = "Approved";
		$xx['status']= "success";       


		AuditTrail::registerTrail("REQUISITION-APPROVALS-COMPLETED", $db_id="",  "REQUISITION", "Requsition with reference no.: $reference created by ".$t->full_name($requestor)." has been fully approved by ".$t->full_name($user_id));
        


	}else{

		// //notifying requestor about the approval
		// $users_emails = array();
		// $users_emails[] = $t->rgf("sysuser", $requestor, "user_id", "user_email");
		// $subject = $type1.' '.$reference.' Approved';
		// $message = "";
		// $message .= "Dear ".$t->full_name($requestor).', <br/><br/>';
		// $message .= "Your ".$type1." with reference No.: <b>".$reference."</b> has been successfully approved by ".$t->full_name(user_id());
		// $message .= "<br/>";
		// $message .= "Its now forwarded to ".$t->rgf("approval_matrix", $forwarded, "ap_id", "ap_unit_code");
		// FeedBack::sendmailz($users_emails,$subject,$message,$hod_name);


		//notifying the next approval about the pending approval
		$emails = array();
		$names = array();
		$db = new Db();

		if(1){
			$sql = "SELECT app_role_id,ap_code, ap_unit_code FROM approval_matrix, master_approval_orders WHERE app_role_id = ap_id AND app_type = 'WORKFLOWONE' AND app_count =  '".($count+1)."' ORDER BY app_count ASC";
		}

		$select = $db->select($sql);
		if($db->num_rows()){
		  $i=0;

	  foreach($select as $row){
	    extract($row);

	    $sql = "SELECT apg_branch, user_email, user_id FROM approval_group,sysuser WHERE apg_user = user_id AND apg_name = '$app_role_id'";
	    $db2 = new Db();
	    $select = $db2->select($sql);
	      $x = array();
	      foreach($select as $row){
	        extract($row);

	       {

		        $emails[] = $user_email;
		        $names[] = $t->full_name($user_id);
			}
	      }
	    }
	    $i++;
	  }




		$subject = $type1.' '.$reference.' Pending Approval';
		$message = "";
		//$message .= "Dear ".$t->rgf("approval_matrix", $forwarded, "ap_id", "ap_unit_code").', <br/><br/>';
		//$message .= "You have ".$type1." with reference No.: <b>".$reference."</b> pending to be approved ";

		$link = return_url()."requisition/view-requisition/".$id; 
		$rg = $t->rgf("requisition", $id, "req_id","req_number");

		$message .= "Dear ".$t->rgf("approval_matrix", $forwarded, "ap_id", "ap_unit_code")." (".$bbranch.implode(', ', $names)."),";
		$message .= "\r\n<br/><br/>You have a pending requisition with No.: <b>$rg</b>\r\n<br/><br/> ";
		$message .= "\r\nYou can use this link: <a href='$link'>$link</a>";

		// FeedBack::sendmailz($emails,$subject,$message,$hod_name);

		AuditTrail::registerTrail("REQUISITION-APPROVAL", $db_id="",  "REQUISITION", "Requsition with reference no.: $reference created by ".$t->full_name($requestor)." has been approved by ".$t->full_name($user). ".");

	}


	//echo $db->error();
	//notify next approver
	$xx['details'] = "Approved";
	$xx['status']= "success";

}else if($choice == "OnHold"){

}else if($choice == "Reject"){

	//this->rejector("requisition", ["req_id"=>$req_id], ["req_app"=>$req]);

	//removing comment
	$n = new Db();
	$item = array();
	$item[$status] = 0;
	$item['oi_counter'] = 0;

	$item2 = array();
	$item2[$id_field] = $id;

	$n->update($table, $item, $item2);


	$n = new Db();
	//$nn = $n->query("DELETE FROM master_approvals WHERE ma_match_id = '$id' AND ma_type = '$type'");

	$nn = $n->insert("master_rejection", [
		"mr_comment"=>$comment,
		"mr_date_added"=>time(),
		"mr_added_by"=>user_id(),
		"mr_rejected_by"=>user_id(),	
		"mr_type_id"=>$id,
		"mr_type"=>$type,
	]);

	
	//notifying requestor about the approval
	$users_emails = array();
	$users_emails[] = $t->rgf("sysuser", $requestor, "user_id", "user_email");
	$subject = $type1.' '.$reference.' Rejected';
	$message = "";
	$message .= "Dear ".$t->full_name($requestor).', <br/>';
	$message .= "Your ".$type1." with reference No.: <b>".$reference."</b> has been rejected by ".$t->full_name(user_id());

	AuditTrail::registerTrail("REQUISITION-REJECTION", $db_id="",  "REQUISITION", "Requsition with reference no.: $reference created by ".$t->full_name($requestor)." has rejected by ".$t->full_name($user_id));

	//FeedBack::sendmailz($users_emails,$subject,$message,$hod_name);

	$xx['message'] = "Success";
	$xx['details'] = "Request Complete";
}

//$xx['details'] = "Rejection complete";
//$xx['status']= "success";

          
echo json_encode($xx);