<?php
error_reporting(null);
include "../classes/init.inc";
$t = new BeforeAndAfter();

			$date = $_POST['date'];
			$category = $_POST['category'];
			$customer = $_POST['customer'];
			$pay_template = $_POST['pay_template'];
			$currency = $_POST['currency'];
			$general_description = $_POST['general_description'];
			$vat = $_POST['vat'];
			$invid = $_POST['invid'];
			$efris_id = $_POST['efris_id'];
			$oi_idid = $_POST['oi_idid'];
			$ref_no = $_POST['ref_no'];
			$source = $_POST['source'];
			//echo''.$general_description.'  '.$currency;
			$time = time();
			$user = user_id();
			$errors = array();

			$totalItems = $_POST['totalItems'];

			$rel = time().user_id();

			$xx = array();
			if(empty($errors)){
				$db = new Db();
			$insert = $db->insert("cn_other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_currency"=>$currency,"oi_general_description"=>$general_description,"oi_added_by"=>$user,"oi_date_added"=>$time,"oi_date"=>strtotime($date),"oi_rel"=>$rel, "oi_vat"=>$vat, "oi_efris_id"=>$efris_id, "oi_idid"=>$oi_idid,"oi_ref_no"=>$ref_no,"oi_source"=>$source]);

				for($i=0; $i<$totalItems; $i++){
					$d = $_POST['description'.$i];
					$q = $_POST['quantity'.$i];
					$a = str_replace(',','',$_POST['amount'.$i]);
					$insert = $db->insert("cn_invoice_item_description",["ii_description"=>$d, "ii_amount"=>$a,"ii_quantity"=>$q,"ii_date_added"=>time(), "ii_added_by"=>user_id(), "ii_rel"=>$rel]);
				}
				
				$xx['message'] = 'Successfully Saved';
        }
        // else{
        	
        //   $xx['message'] = ('Not Saved, '.$db->error());
        // }
		else{
      	$xx['message']='Error';
        
      }
          
echo json_encode($xx);