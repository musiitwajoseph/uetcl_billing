<?php
Class OtherInvoiceCustomers extends BeforeAndAfter{
	public $page = "OTHER INVOICE CUSTOMERS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public static function getLinks(){
		$page = "OTHER INVOICE CUSTOMERS";
		$links = array(
			array(
				"link_name"=>"Add Customer", 
				"link_address"=>"other-invoice-customers/add-customer",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"All Customers", 
				"link_address"=>"other-invoice-customers/all-customers",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Add Category", 
				"link_address"=>"other-invoice-customers/add-category",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			// array(
			// 	"link_name"=>"View all-categories", 
			// 	"link_address"=>"other-invoice-customers/all-categories",
			// 	"link_icon"=>"fa-eye",
			// 	"link_page"=>$page,
			// 	"link_right"=>"V",
			// )
		);
		
		return $links;
	}
	public function addCustomerAction(){
		if(isset($_POST['submit'])){
		
			$category = $_POST['category'];
			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();
			if(empty($category)){
				$errors[]="Enter Category";
			}
			
			
			if(empty($errors)){
				if($id){
					$x = $db->update("categories",["cat_date_added"=>$time, "cat_name"=>$category,"cat_added_by"=>$user],["cat_id"=>$id]);
				}else{
					$x = $db->insert("categories",["cat_date_added"=>$time, "cat_name"=>$category,"cat_added_by"=>$user]);
				}
				
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'other-invoice-customers/add-category');
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Customer Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
									<div class="row">
										<div class="col-md-3" style="display:none;">
											<label>Category Name<span class="must"></span></label>
											<div class="form-line">
											<select id="category" class="form-control">
												<?php
												echo '<option>-----select------</option>';	
												$db = new Db();
												$select = $db->select("SELECT * FROM categories");
												foreach ($select[0] as $row) {
													extract($row);
													echo '<option value="'.$cat_id.'">'.$cat_name.'</option>';	
												}

												?>
											</select>
										</div>
									  </div>
									  <div class="col-md-3">
										<label>Customer Name<span class="must"></span></label>
										<div class="form-line">
										  <input type="text" id="customer_name" class="form-control" placeholder="Customer Name">
									   </div>
									  </div>
									  <div class="col-md-3">
										<label>Telephone<span class="must"></span></label>
										<div class="form-line">
										  <input type="text" id="telephone" class="form-control" placeholder="Customer Telephone">
									   </div>
									  </div>
									  <div class="col-md-3">
										<label>Email<span class="must"></span></label>
										<div class="form-line">
										  <input type="email" id="email_addres" class="form-control" placeholder="Customer Email">
									   </div>
									  </div>
									  <div class="col-md-3">
										<label>Plot & Street<span class="must"></span></label>
										<div class="form-line">
										  <input type="text" id="street" class="form-control" placeholder="Plot & Street">
									   </div>
									  </div>
									  <div class="col-md-3">
										<label>TIN<span class="must"></span></label>
										<div class="form-line">
										  <input type="text" id="tin_number" class="form-control" placeholder="TIN">
									   </div>
									  </div>
									  <div class="col-md-3">
										<label>Customer Type<span class="must"></span></label>
										<div class="form-line">
										  <select class="form-control" id="cust_type">
										  	<option value="B2B">B2B</option>
										  	<option value="B2G">B2G</option>
										  	<option value="B2C">B2C</option>
										  	<option value="B2F">B2F</option>
										  </select>
									   </div>
									  </div>
								</div>
								<div class="clearfix"></div>
								<br>
								<button id="addCustomer" type="button" name="submit" class="pull-left btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>

								<button id="validate_tin" type="button" name="submit" class="pull-right btn btn-primary"><i class="fa fa-fw fa-check"></i> Validate Tin</button>
								<div class="clearfix"></div>
								<span id="CustomerStatus"></span>
								
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
	<script type="text/javascript">		
		var urlPath = $('#urlPath').val();

		$(document).off('click','#auto_fill').on('click', '#auto_fill', function(e){
			//$('#CustomerStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');	
			var values = $("#values").val();
			alert(values);
			values = values.split('@@@');
			$('#street').val("sdfsdf");
			$('#customer_name').val(values[1]);
			$('#telephone').val(values[3]);
			$('#email_addres').val(values[2]);
			$('#cust_type').html('<option value="B2B" selected>B2B</option>');
			$('#CustomerStatus').html('Content Auto Filled');	
			
		});

		$('#validate_tin').click(function(){
			$('#CustomerStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');	
			$(this).attr("disabled", true);

			var form_data = new FormData();					
			form_data.append('tin', $('#tin_number').val());

			$.ajax({
				url: urlPath+"ajax/pushValidateTIN.php",
				type: "POST",
				data: form_data,
				contentType: false,
				cache: false,
				processData:false,
				success: function(data){	
					var	values = JSON.parse(data);
					if(values.status == "error"){
						$('#CustomerStatus').html(values.error);
					}else{
						$('#CustomerStatus').html('<b>Validated</b>');
						//location.reload();							
						$('#CustomerStatus').append(values.response);							
					}

					$("#validate_tin").attr("disabled", false);
				}
			});
		});

		$('#addCustomer').click(function(){
			$('#CustomerStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
			$(this).attr("disabled", true);
		
			// if(empty($customer_name)){
   //          $errors[] = "Enter Customer Name";
   //         }
			if(1){
				//alert( $('#OperationType').val());
				var form_data = new FormData();				
				var form_data = new FormData();				

				form_data.append('customer_name', $('#customer_name').val());				
				form_data.append('category', 1);
				form_data.append('telephone', $('#telephone').val());
				form_data.append('street', $('#street').val());
				form_data.append('tin_number', $('#tin_number').val());
				form_data.append('cust_type', $('#cust_type').val());
				form_data.append('email_addres', $('#email_addres').val());
				
				$.ajax({
					url: urlPath+"ajax/addCustomer.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){			
						//alert(data);
						var	values = JSON.parse(data);
						$('#addCustomer').attr("disabled",false);
						if(values.status == "error"){
							$('#CustomerStatus').html(values.error);
						}else{
							$('#CustomerStatus').html('Saved');
							location.reload();							
							$('#CustomerStatus').append(values.response);							
						}
					}
				});
			}else{
				//alert("Enter Lot No.");
				$('#CustomerStatus').html("");
			}
		});
	</script>
	<?php
	}
	public function viewCustomerDetailsAction(){
		$id =  portion(3);
		$id2 =  portion(4);
		$db=new Db();
		$select=$db->select("select * from customers WHERE cust_id='$id'");
		if($db->num_rows()){
		extract($select[0][0]);
	    }
	    
		if($id2){	
		 $db = new Db();	
		  $ss = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='OTHERINVOICES' AND gl_id='$id2'");
		  if($db->num_rows()){
		  	extract($ss[0][0]);
		  }
		}
	 	echo'<br>';
		echo'<table>';
		echo'<table>';           
            echo'<tr>';
                echo'<td width="130px"><b>Customer Name:</b></td>';
                echo'<td>'.$cust_name.'</td>';
            echo'</tr>';
            // echo'<tr>';
            //     echo'<td width=""><b>Category:</b></td>';
            //     echo'<td>'.$this->rgf("categories", $cust_category,"cat_id","cat_name").'</td>';
            // echo'</tr>';          
            echo'<tr>';
                echo'<td width=""><b>Telephone:</b></td>';
                echo'<td>'.$cust_telephone.'</td>';
            echo'</tr>';
             echo'<tr>';
                echo'<td width=""><b>Email:</b></td>';
                echo'<td>'.$cust_email.'</td>';
            echo'</tr>';
             echo'<tr>';
                echo'<td width=""><b>Plot & Street:</b></td>';
                echo'<td>'.$cust_street.'</td>';
            echo'</tr>';
            echo'<tr>';
                echo'<td width=""><b>TIN:</b></td>';
                echo'<td>'.$cust_tin.'</td>';
            echo'</tr>';
             echo'<tr>';
                echo'<td width=""><b>Customer Type:</b></td>';
                echo'<td>'.$cust_type.'</td>';
            echo'</tr>';
             echo'<tr>';
                echo'<td width=""><b>Date Addded</b></td>';
                echo'<td>'.FeedBack::date_fm($cust_date_added).'</td>';
            echo'</tr>';
        echo'</table>'; 
        echo "<h2>GL ACCOUNT</h2>";

        ?>
        	<div class="row">
				<div class="col-md-3">
					<div class="form-group">
						<label class="form-label">GL Account</label>
						<select class="select2 form-control" style="width: 100%" id="gl_accounts">
							<option value="">select</option>
							<?php 
							$db = new Db();
		      //               $select = $db->select("SELECT * FROM dictionary WHERE d_category = '9'");
								// foreach ($select[0] as $k) {
								// 	extract($k);
								// 	if($gl_account == $d_id)
								// 	echo'<option selected value="'.@trim($d_id).'">'.@trim($d_description).' ('.@trim($d_code).')</option>';
								// else
								// 	echo'<option value="'.@trim($d_id).'">'.@trim($d_description).' ('.@trim($d_code).')</option>';

								// }

								 $select = $db->select("SELECT * FROM dictionary WHERE d_category IN (9,10,11,12,13,14,15) order by d_code ASC");
								foreach ($select[0] as $k) {
									extract($k);
									if($gl_account == $d_id)
									echo'<option selected value="'.@trim($d_id).'">'.@trim($d_code).' - '.@trim($d_description).' ('.@trim($d_code).')</option>';
								else
									echo'<option value="'.@trim($d_id).'">'.@trim($d_code).' - '.@trim($d_description).' ('.@trim($d_code).')</option>';

								}
							?>
						</select>
					</div>
				</div>
				<script type="text/javascript">
					   $('.select2').select2();
				</script>
				
				<div class="col-md-2">
					<div class="form-group">
						<label class="form-label">GL Label</label>
						<select class="form-control" id="gl_label">
							<option value="">--select---</option>
							<?php 
							$db = new Db();
		                    $select = $db->select("SELECT * FROM dictionary WHERE d_category = '12'");
								foreach ($select[0] as $k) {
									extract($k);
									if($d_id == $gl_label)
									echo'<option selected value="'.$d_id.'">'.$d_description.'</option>';
								else
									echo'<option value="'.$d_id.'">'.$d_description.'</option>';

								}
							?>
						</select>
					</div>
				</div>
				<div class="col-md-2">
					<div class="form-group">
						<label class="form-label">Ct/Dt</label>
						<select class="select3 form-control" id="ct_dt">
							<option value="">--select---</option>
							<option <?php if($gl_ct_bt=='Credit') echo "selected";?> value="Credit">Credit</option>
							<option <?php if($gl_ct_bt=='Debit') echo "selected";?> value="Debit">Debit</option>
						</select>
					</div>
				</div>
				<div class="col-md-12">
					<input type="hidden" id="cust_id" value="<?php echo $id;?>" name="">
					<input type="hidden" id="account_id" value="<?php echo $id2;?>" name="">

					<?php
					if($id2){
						$save_name ="Update";
					}else{
						$save_name ="Save";
					}
					?>
				<button class="btn btn-primary btn-xs" id="save_gl_account"><i class="fa fa-save"></i> <?php echo $save_name;?></button><span id="save_gl_account_status"></span>
					<script type="text/javascript">
				        $(document).off('click','#save_gl_account').on('click', '#save_gl_account', function(e){

				        var gl_accounts = $('#gl_accounts').val();
				        var gl_label = $('#gl_label').val();
				        var ct_dt = $('#ct_dt').val();
				        var cust_id = $('#cust_id').val();
				        var account_id = $('#account_id').val();
				        

				        var errors = new Array(); 
				        if(gl_accounts==""){
				            errors.push("Please Select GL Account");
				        }
				        if(gl_label==""){
				            errors.push("Please Select GL Label");
				        }
				         if(ct_dt==""){
				            errors.push("Please Select Ct/Dt");
				        }
				        

				        if(errors.length >= 1){
				            alert("Please Review the following:\n "+errors.join("\n"));
				        }else{
				            $(this).html("Adding Account");
				            $(this).attr("disabled", true);
				            var form_data = new FormData();
				            form_data.append('gl_accounts', gl_accounts);
				            form_data.append('gl_label', gl_label);
				            form_data.append('ct_dt', ct_dt);
				            form_data.append('cust_id', cust_id);
				            form_data.append('account_id', account_id);
				            form_data.append('account_entity', 'OTHERINVOICES');
				           

				            $.ajax({
				            url:"<?php echo return_url().'ajax/gl_accounts.php';?>",
				            type: "POST",
				            data: form_data,
				            contentType: false,
				            cache: false,
				            processData:false,

				            success: function(data){
				                //alert(data);
				                var values = JSON.parse(data);
				                if (values.message=='Error') {
				                    $('#save_gl_account').html("Adding Account");
				                    $('#save_gl_account').attr('disabled',false);
				                    $('#save_gl_account_status').html('Not Added');
				                }else{
				                    //location.reload();customers/energy-customer-details/201
				                     window.location.replace("<?php echo return_url().'other-invoice-customers/view-customer-details/'.$id;?>");
				                    $('#save_gl_account_status').html('Successfully Saved');
				                }
				            }
				        
				        }); 
				        }

				    });
				    </script>       
			   
			    <?php
			    echo "<br><br>";
			    $select = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='OTHERINVOICES'");
			    if($select){
			    	echo '<table id="table2" border="1">';
					echo '<thead>';
					echo '<tr>';
					echo '<th width="30px">No.</th>';
					echo '<th>GL Account</th>';
					echo '<th>GL Label</th>';
					echo '<th>Ct/Dt</th>';
					echo '<th style="width:150px;">Action</th>';
					echo '</tr>';
					echo '</thead>';
					$no = 1;
					foreach($select[0] as $row){
						extract($row);
						echo '<tr>';
						echo '<td>'.($no++).'.</td>';
						echo '<td>'.strtoupper($this->rgf("dictionary", $gl_account, "d_id", "d_name")).' (<b>'.strtoupper($this->rgf("dictionary", $gl_account, "d_id", "d_code")).'</b>)</td>';
						echo '<td>'.strtoupper($this->rgf("dictionary", $gl_label, "d_id", "d_name")).'</td>';
						echo '<td>'.$gl_ct_bt.'</td>';
						
						echo '<td style="text-align:right">
						<a href="'.return_url().'other-invoice-customers/delete-gl-account/'.$id.'/'.$gl_id.'" class="btn btn-danger btn-xs"><i class="fa fa-fx fa-times"></i> Delete</a> &nbsp;
						<a href="'.return_url().'other-invoice-customers/view-customer-details/'.$id.'/'.$gl_id.'" class="eagle-load1 btn btn-success btn-xs"><i class="fa fa-fx fa-edit"></i> Edit</a></td>';
						echo '</tr>';
					}

					echo '</table>';
				}
			    ?>
			     </div>
			</div>
			<?php
		echo '</div>';
		echo '</div>';  
	}
	public function deleteGLAccountAction(){
		$id = portion(3);
		$id2 = portion(4);
		$db = new Db();
		$sql = "DELETE FROM gl_account WHERE gl_id = '$id2'";
		$delete = $db->query($sql);
		FeedBack::redirect(return_url().'other-invoice-customers/view-customer-details/'.$id);
	}
	public function AllCustomersAction(){
	$access = new AccessRights();
	?>
		<div class="col-md-12">
			
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM customers ORDER BY cust_name ASC");
			
			if(!$select){
				$db->error();
			}else{
				 echo '<div class="pull-right" style="margin:0 10px;">';
            echo '<a href="'.return_url().'classes/export-data.php?report_type=Customers"><i class="fa fa-fw fa-list"></i> Export to Excel</a>';
        echo '</div>';
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Customer Name</th>';
				//echo '<th>Category</th>';
				echo '<th>Telephone</th>';
				echo '<th>Email</th>';
				echo '<th>Plot & Street</th>';
				echo '<th>Tin</th>';
				echo '<th>Type</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="140px">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.($cust_name).'</td>';
					//echo '<td>'.$this->rgf("categories",$cust_category,"cat_id","cat_name").'</td>';
					echo '<td>'.($cust_telephone).'</td>';
					echo '<td>'.($cust_email).'</td>';
					echo '<td>'.($cust_street).'</td>';
					echo '<td>'.($cust_tin).'</td>';
					echo '<td>'.($cust_type).'</td>';
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo $this->action('edit','other-invoice-customers/edit-customer/'.$cust_id, 'Edit');
						if($access->sectionAccess(user_id(), $this->page, 'D'))
							echo $this->action('delete','other-invoice-customers/delete-customer/'.$cust_id, 'Delete');
						echo '&nbsp;&nbsp;<a class="btn btn-xs btn-danger" onclick="return confirm(\'Do you want to delete this customer?\');" href="'.return_url().'other-invoice-customers/delete-customer/'.$cust_id.'"> Delete</a>&nbsp;&nbsp;';
							echo "&nbsp;<a href='".return_url().'other-invoice-customers/view-customer-details/'.$cust_id."' class='btn btn-default btn-xs'>View</a>";
						echo '</td>';
					}
					echo '</tr>';
					
				}
				echo '</tbody>';
				
				echo '</table>';
				
			}
			
			?>
		</div>
		<?php
	}
	public function deleteCustomerAction(){
		$id = portion(3);
		$this->deletor("customers", "cust_id",  $id, 'other-invoice-customers/all-customers');
	}
	public function deleteCategoryAction(){
		$id = portion(3);
		$this->deletor("categories", "cat_id",  $id, 'other-invoice-customers/add-category');
	}
	
	public function addCategoryAction(){
		$id=portion(3);
		$db=new Db();
		if($id){
		$select=$db->select("select * from categories WHERE cat_id='$id'");
		if($db->num_rows()){
		extract($select[0][0]);
	    }
	   }
		if(isset($_POST['submit'])){
		
			$category = $_POST['category'];
			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();
			if(empty($category)){
				$errors[]="Enter Category";
			}
			
			
			if(empty($errors)){
				if($id){
					$x = $db->update("categories",["cat_date_added"=>$time, "cat_name"=>$category,"cat_added_by"=>$user],["cat_id"=>$id]);
				}else{
					$x = $db->insert("categories",["cat_date_added"=>$time, "cat_name"=>$category,"cat_added_by"=>$user]);
				}
				
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'other-invoice-customers/add-category');
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-4">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Category Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
									<label>Category Name<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" name="category" placeholder="Enter Category Name" value="<?php if($id)echo$cat_name; else echo @$category; ?>">
									</div>
								</div>
								
								<button onclick = "return confirm('Do you really want to save?');" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
	<div class="col-md-1">
	</div>	
	<div class="col-md-7">
			
			<?php
			$access = new AccessRights();
			$db = new Db();
			$select = $db->select("SELECT * FROM categories");
			//print_r($select);
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Category Name</th>';
				
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="100px">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				echo '<tbody>';
				$i = 1;
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.($cat_name).'</td>';
					
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo $this->action('edit','other-invoice-customers/add-category/'.$cat_id, 'Edit');
						if($access->sectionAccess(user_id(), $this->page, 'D'))
							echo $this->action('delete','other-invoice-customers/delete-category/'.$cat_id, 'Delete');
						echo '</td>';
					}
					echo '</tr>';
		
				}
				echo '</tbody>';
				
				echo '</table>';
			
				
			}
			
			?>
		</div>	
	<?php
	}

	
	
}