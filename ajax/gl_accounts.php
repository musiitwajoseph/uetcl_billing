<?php
error_reporting(null);
include("../classes/init.inc");	
$t = new BeforeAndAfter();
			$gl_accounts = $_POST['gl_accounts'];
			$gl_label = $_POST['gl_label'];
			$ct_dt = $_POST['ct_dt'];
			$cust_id = $_POST['cust_id'];
			$account_id = $_POST['account_id'];
			$account_entity = $_POST['account_entity'];
			
			$time = time();
			$user = user_id();
			$errors = array();
			$xx = array();
			if(empty($errors)){
				$db = new Db();
				if(!$account_id){
				 $db->insert("gl_account",[
				 	"gl_date_added"=>$time,
				 	"gl_added_by"=>$user,
				 	"gl_account"=>$gl_accounts,
				 	"gl_label"=>$gl_label,
				 	"gl_ct_bt"=>$ct_dt,
				 	"gl_cust_id"=>$cust_id,
				 	"gl_entity"=>$account_entity,
				 ]);
				}else{
					 $db->update("gl_account",[
				 	"gl_date_added"=>$time,
				 	"gl_added_by"=>$user,
				 	"gl_account"=>$gl_accounts,
				 	"gl_label"=>$gl_label,
				 	"gl_ct_bt"=>$ct_dt,
				 	"gl_cust_id"=>$cust_id,
				 	"gl_entity"=>$account_entity,
				 ],["gl_id"=>$account_id]);
				}	
				 echo $db->error();
				
				
				$xx['message'] = 'Successfully Saved';
        }
        //else{
        	
        //   $xx['message'] = ('Not Saved, '.$db->error());
        // }
		else{
      	$xx['message']='Error';
        
      }
          
echo json_encode($xx);