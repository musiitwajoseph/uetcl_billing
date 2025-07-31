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
			$id = $_POST['id'];
			$vat = $_POST['vat'];
			$ref_no = $_POST['ref_no'];
			$time = time();
			$user = user_id();
			$errors = array();
			$totalItems = $_POST['totalItems'];

			$db = new Db();
			$rel = $t->rgf("other_invoice", $id, "oi_id", "oi_rel");
			$db->query("DELETE FROM invoice_item_description WHERE ii_rel = '$rel'");

			$db = new Db();
			$xx = array();
			if(empty($errors)){
				$insert = $db->update("other_invoice",["oi_category"=>$category,"oi_customer"=>$customer,"oi_pay_template"=>$pay_template,"oi_currency"=>$currency,"oi_general_description"=>$general_description,"oi_added_by"=>$user,"oi_date_added"=>$time,"oi_date"=>strtotime($date),"oi_rel"=>$rel,"oi_vat"=>$vat,"oi_ref_no"=>$ref_no],["oi_id"=>$id]);
				
				$xx['message'] = 'Successfully Saved';

				for($i=0; $i<$totalItems; $i++){
					$d = $_POST['description'.$i];
					$q = $_POST['quantity'.$i];
					$a = str_replace(',','',$_POST['amount'.$i]);
					$insert = $db->insert("invoice_item_description",["ii_description"=>$d, "ii_amount"=>$a,"ii_quantity"=>$q,"ii_date_added"=>time(), "ii_added_by"=>user_id(), "ii_rel"=>$rel]);
				}

				$xx['id']=$id;
        }
        //else{
        	
        //   $xx['message'] = ('Not Saved, '.$db->error());
        // }
		else{
      	$xx['message']='Error';
        
      }
          
echo json_encode($xx);