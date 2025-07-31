<?php
error_reporting(null);
include "../classes/init.inc";
$t = new BeforeAndAfter();
			$date = $_POST['date'];
			$receipt_no = $_POST['receipt_no'];
			$category = $_POST['category'];
			$customer_name = $_POST['customer_name'];
			$amount = $_POST['amount'];
			$description = $_POST['description'];
			$curr = $_POST['curr'];
			$vat_status = $_POST['vat_status'];
			$section = trim($_POST['section']);
			$project = trim($_POST['project']);
			$staff = trim($_POST['staff']);
			$bank_id = $_POST['account_name'];
			$new = $_POST['new_customer'];
			$account2 = $_POST['account2'];
			$new_account2 = $_POST['new_account2'];
			$new_account22 = $_POST['new_account22'];
			$reference = $_POST['reference'];
			$time = time();
			$user = user_id();
			$errors = array();
			$totalItems = $_POST['totalItems'];

			$db = new Db();
			//$rel = $t->rgf("dn_other_invoice", $id, "oi_id", "oi_rel");
			//$db->query("DELETE FROM dn_invoice_item_description WHERE ii_rel = '$rel'");

			$db = new Db();
			$xx = array();
			if(empty($errors)){
				if(!empty($new_account2)){
					//insert into the items table				
					$insert = $db->insert("account2", ["account_name"=>$new_account2,"account_number"=>$new_account22, "account_date_added"=>time(), "account_added_by"=>user_id()]);
					$uid = user_id();
					$select = $db->select("SELECT TOP 1 account_id as ac2 FROM account2 WHERE account_added_by='$uid' ORDER BY account_id DESC");
					extract($select[0][0]);
					$account2 = $ac2;
				}
				
				
				if(!empty($new)){
					//insert into the items table				
					$insert = $db->insert("receipt_category_item", ["rci_name"=>$new, "rci_date_added"=>time(), "rci_added_by"=>user_id(), "rci_category"=>$category]);
					
					$select = $db->select("SELECT TOP 1 rci_id FROM receipt_category_item ORDER BY rci_id DESC");
					extract($select[0][0]);
					$customer_name = $rci_id;
				}
				
				$ref =  time().user_id();
				$user_id = user_id();

				
			
				$db = new Db();
				$am = str_replace(',', '', $amount);
				$cus =  "";//$this->rgf("customer", $customer_name, "customer_id", "customer_short_name");
				$x = $db->update("receipts",["rec_date_added"=>$time, "rec_added_by"=>user_id(),"rec_category"=>$category, "rec_receipt_no"=>$receipt_no.''.$cus, "rec_amount"=>$am, "rec_customer_id"=>$customer_name, "rec_description"=>$description, "rec_date"=>strtotime($date), "rec_vat_status"=>$vat_status, "rec_staff"=>$staff, "rec_project"=>$project, "rec_section"=>$section, "rec_curr"=>$curr, "rec_bank"=>$bank_id, "rec_account2"=>$account2],["rec_ref"=>$reference]);
				
				$xx['message'] = 'Successfully Saved';

				$db->query("DELETE FROM receipt_amount WHERE ra_ref='$reference'");
				for($i=0; $i<$totalItems; $i++){
					$d = $_POST['description'.$i];
					$a = str_replace(',','',$_POST['amount'.$i]);
					$insert = $db->insert("receipt_amount",["ra_desc"=>$d, "ra_amount"=>$a,"ra_date_added"=>time(), "ra_added_by"=>user_id(), "ra_ref"=>$reference]);
				}
        }
        //else{
        	
        //   $xx['message'] = ('Not Saved, '.$db->error());
        // }
		else{
      	$xx['message']='Error';
        
      }
          
echo json_encode($xx);