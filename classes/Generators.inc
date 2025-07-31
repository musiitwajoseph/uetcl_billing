<?php
Class Generators extends BeforeAndAfter{
	public	$page = "ENERGY PURCHASES";
	
	public function __construct(){
		$access = new AccessRights();
		$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "ENERGY PURCHASES";
		$links = array(
			array(
				"link_name"=>"Add Generator", 
				"link_address"=>"generators/add-generator",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"bill", 
				"link_address"=>"generators/bill",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Add Purchase Transactions", 
				"link_address"=>"generators/add-purchase-transaction",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Vie Purchase Transactions", 
				"link_address"=>"generators/view-purchase-transactions",
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}
	
	public function addGenerator(){
		echo '<h4>ADD GENERATOR</h4>';
		if(isset($_POST['submit'])){
			
			$metering_point = $_POST['metering_point'];
			$generator_category = $_POST['generator_category'];
			$generator_currency = $_POST['generator_currency'];


			$time = time();
			$user = user_id();

			$db = new Db();
			
			$errors = array();

			if(empty($metering_point)){
				$errors[]="Enter Generator name";
			}
			if(empty($generator_category)){
				$errors[]="Select Generator Gategory";
			}
			
			if(empty($generator_currency)){
				$errors[]="Select Bill Currency";
			}
			

			if(empty($errors)){
				$db = new Db();

				$x = $db->insert("generators",
					[
						"ge_name"=>$metering_point,
					 	"ge_category"=>$generator_category,
					 	"ge_currency"=>$generator_currency,
					 	"ge_date_added"=>time(),
					 	"ge_added_by"=>user_id(),
					]);
				//FeedBack::error($db->error());
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'generators/add-generator');
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Generator Form
				</div>
				<div class="panel-body">
					
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-3">
										<div class="form-group">
											<label>Name<span class="must">*</span></label>
											<div class="form-line">
												<input type="text" name="metering_point" value="<?php echo $metering_point; ?>"/>
												
											</div>
										</div>
										<!-- Collectable from Metering Point -->
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Generator<span class="must">*</span></label>
											<div class="form-line">
												<select name="generator_category">
													<option value=""> Select</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM generator_category ORDER BY gc_name ASC");

													foreach($select[0] as $row){
														extract($row);

														if(1){		
															echo '<option value="'.$gc_id.'">'.$gc_name.'</option>';
														}
													}
													?>
												</select>
											</div>
										</div>
										<!-- Collectable from Metering Point -->
									</div>

									<div class="col-lg-2">
										<div class="form-group">										
											<label>Bill Currency<span class="must"></span></label>
											<div class="form-line">
												<select name="generator_currency">
													<option value=""> Select</option>
													<?php

													$currency = array(
														'USD'=>'Dollars', 
														'UGX'=>'Shillings',
														'EUR'=>'Euro', 
													);

													foreach($currency as $row=>$name){
														extract($row);
														if(1){		
															echo '<option value="'.$row.'">'.$name.'</option>';
														}
													}
													?>
												</select>
											</div>
										</div>
									</div>

									<div class="col-lg-2">
										<div class="form-group">										
											<label><span class="must"></span></label>
											<div class="form-line">
												<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
											</div>
										</div>
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>

			<input type="text" style="padding:0 10px; margin-bottom: 10px;" name="search" autocomplete="off" placeholder="Search Metering Point" id="searchTerm" onkeyup="doSearch2()" onClick="this.select();">

			<table id="table2" border="1">
				<thead>
					<tr valign="bottom">
						<th width="40px">No.</th>
						<th>Generator Name</th>
						<th>Generator Category</th>
						<th>Bill Currency</th>
						<th>Action</th>
					</tr>
				</thead>
				<tbody>
				<?php 
				$db = new Db();
				$select = $db->select("SELECT * FROM generators ORDER BY ge_category ASC");
				$g = 0;
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
						echo '<td>'.(++$g).'.</td>';
						echo '<td>'.$ge_name.'</td>';
						echo '<td>'.strtoupper($this->rgf("generator_category", $ge_category, "gc_id", "gc_name")).'</td>';
						echo '<td>'.$ge_currency.'</td>';
						
						echo '<td><a onclick="return confirm(\'Do you want to delete '.$ge_name.'\');" href="'.return_url().'generators/delete-generator/'.$ge_id.'"><i class="fa fa-fw fa-times"></i>&nbsp; Delete</a> &nbsp;&nbsp;&nbsp; <a href="'.return_url().'generators/edit-generator/'.$ge_id.'"><i class="fa fa-fw fa-edit"></i> Edit</a> &nbsp; <a href="'.return_url().'generators/add-gl-account/'.$ge_id.'"><i class="fa fa-fw fa-plus"></i> Add GL Account</a></td>';						
					echo '</tr>';
				}

				?>
				</tbody>

			</table>
		</div>
	<?php
	}
public function addGlAccountAction(){
		$id =  portion(3);
		$id2 =  portion(4);
		if($id2){	
		 $db = new Db();	
		  $ss = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='PURCHASES' AND gl_id='$id2'");
		  if($db->num_rows()){
		  	extract($ss[0][0]);
		  }
		}
		$db = new Db();
		$select = $db->select("SELECT * FROM generators WHERE ge_id = '$id'");
		extract($select[0][0]);
		
		echo "<h2>GL ACCOUNT</h2>";
		echo '<style>#y tr td{padding:10px;}</style>';
		echo '<table id="y">';

		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Generator Name:</b></td>';
		echo '<td>'.$ge_name.'</td>';
		echo '</tr>';
		
		echo '<tr valign="top">';
		echo '<td style="width:200px;"><b>Category:</b></td>';
		echo '<td>'.strtoupper($this->rgf("generator_category", $ge_category, "gc_id", "gc_name")).'</td>';
		echo '</tr>';
		
		echo '</table>';
		echo"<br><br>";
			?>
			<div class="row">
				<div class="col-md-3">
					<div class="form-group">
						<label class="form-label">GL Account</label>
						<select class="select2 form-control" style="width: 100%" id="gl_accounts">
							<option value="">select</option>
							<?php 
							$db = new Db();
		                    $select = $db->select("SELECT * FROM dictionary WHERE d_category = '9'");
								foreach ($select[0] as $k) {
									extract($k);
									if($gl_account == $d_id)
									echo'<option selected value="'.@trim($d_id).'">'.@trim($d_description).' ('.@trim($d_code).')</option>';
								else
									echo'<option value="'.@trim($d_id).'">'.@trim($d_description).' ('.@trim($d_code).')</option>';

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
				            form_data.append('account_entity', 'PURCHASES');
				           

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
				                     window.location.replace("<?php echo return_url().'generators/add-gl-account/'.$id;?>");
				                    $('#save_gl_account_status').html('Successfully Saved');
				                }
				            }
				        
				        }); 
				        }

				    });
				    </script>        
			   
			    <?php
			    echo "<br><br>";
			    $select = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='PURCHASES'");
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
						<a href="'.return_url().'generators/delete-gl-account/'.$id.'/'.$gl_id.'" class="btn btn-danger btn-xs"><i class="fa fa-fx fa-times"></i> Delete</a> &nbsp;
						<a href="'.return_url().'generators/add-gl-account/'.$id.'/'.$gl_id.'" class="eagle-load1 btn btn-success btn-xs"><i class="fa fa-fx fa-edit"></i> Edit</a></td>';
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
		FeedBack::redirect(return_url().'generators/add-gl-account/'.$id);
	}
	public function editGenerator(){
		echo '<h4>EDIT GENERATOR</h4>';
		$id = portion(3);

		$db = new Db();
		$select = $db->select("SELECT * FROM generators WHERE ge_id = '$id' ");
		extract($select[0][0]);



		$metering_point = $ge_name;
		$generator_category = $ge_category;
		$generator_currency = $ge_currency;

		if(isset($_POST['submit'])){
			
			$metering_point = $_POST['metering_point'];
			$generator_category = $_POST['generator_category'];
			$generator_currency = $_POST['generator_currency'];


			$time = time();
			$user = user_id();

			$db = new Db();
			
			$errors = array();

			if(empty($metering_point)){
				$errors[]="Enter Generator name";
			}
			if(empty($generator_category)){
				$errors[]="Select Generator Gategory";
			}
			
			if(empty($generator_currency)){
				$errors[]="Select Bill Currency";
			}
			

			if(empty($errors)){
				$db = new Db();

				$x = $db->update("generators",
					[
						"ge_name"=>$metering_point,
					 	"ge_category"=>$generator_category,
					 	"ge_currency"=>$generator_currency,
					 	"ge_date_added"=>time(),
					 	"ge_added_by"=>user_id(),
					], ["ge_id"=>$id]);
				//FeedBack::error($db->error());
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'generators/add-generator');
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Generator Form
				</div>
				<div class="panel-body">
					
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-3">
										<div class="form-group">
											<label>Name<span class="must">*</span></label>
											<div class="form-line">
												<input type="text" name="metering_point" value="<?php echo $metering_point; ?>"/>
												
											</div>
										</div>
										<!-- Collectable from Metering Point -->
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Generator<span class="must">*</span></label>
											<div class="form-line">
												<select name="generator_category">
													<option value=""> Select</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM generator_category ORDER BY gc_name ASC");

													foreach($select[0] as $row){
														extract($row);

														if($generator_category == $gc_id){		
															echo '<option selected="selected" value="'.$gc_id.'">'.$gc_name.'</option>';
														}else{		
															echo '<option value="'.$gc_id.'">'.$gc_name.'</option>';
														}
													}
													?>
												</select>
											</div>
										</div>
										<!-- Collectable from Metering Point -->
									</div>

									<div class="col-lg-2">
										<div class="form-group">										
											<label>Bill Currency<span class="must"></span></label>
											<div class="form-line">
												<select name="generator_currency">
													<option value=""> Select</option>
													<?php

													$currency = array(
														'USD'=>'Dollars', 
														'UGX'=>'Shillings',
														'EUR'=>'Euro', 
													);

													foreach($currency as $row=>$name){
														//extract($row);
														if($generator_currency==$row){		
															echo '<option selected="selected" value="'.$row.'">'.$name.'</option>';
														}else{		
															echo '<option value="'.$row.'">'.$name.'</option>';
														}
													}
													?>
												</select>
											</div>
										</div>
									</div>

									<div class="col-lg-2">
										<div class="form-group">										
											<label><span class="must"></span></label>
											<div class="form-line">
												<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
											</div>
										</div>
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

<?php

}


	public function deleteGeneratorAction(){
		$id = portion(3);
		$db = new Db();
		$sql = "DELETE FROM generators WHERE ge_id = '$id'";
		$delete = $db->query($sql);

		FeedBack::redirect(return_url().'generators/add-generator');
	}

	public function addPurchaseTransactionAction() {
		?>
		<div class="container-fluid">
			<div class="row">
				<form>

					<div class="col-md-4">
						<label>FDN</label>
						<input type="text" id="fdn" class="form-control"/>
					</div>

					<div class="col-md-2">
						<label>Transaction Type</label>
						<?php $types = ['INVOICE', 'CREDIT NOTE', 'DEBIT NOTE']; ?>
						<select id="transaction_type" class="form-control">
							<?php foreach ($types as $type): ?>
								<option value="<?= $type ?>"><?= $type ?></option>
							<?php endforeach; ?>
						</select>
					</div>

					<div class="col-md-2">
						<label>Year</label>
						<select name="year" id="year" class="form-control">
							<option>Select</option>
							<?php
							for ($i = 2019; $i <= date('Y'); $i++) {
								$selected = ($i == ($year_uploaded ?? date('Y'))) ? 'selected' : '';
								echo "<option value=\"$i\" $selected>$i</option>";
							}
							?>
						</select>
					</div>

					<div class="col-md-2">
						<label>Month</label>
						<select name="month" id="month" class="form-control">
							<option>Select</option>
							<?php
							for ($i = 1; $i <= 12; $i++) {
								$selected = ($i == ($month_uploaded ?? date('m'))) ? 'selected' : '';
								echo "<option value=\"$i\" $selected>" . month_name($i) . "</option>";
							}
							?>
						</select>
					</div>

					<div class="col-md-12 mt-3">
						<label>Generator</label>
						<select name="generator" id="generator" class="form-control">
							<option>Select</option>
							<?php
							$db = new Db();
							$generators = $db->select('SELECT * FROM generators ORDER BY ge_name ASC');
							foreach ($generators[0] as $generator) {
								extract($generator);
								echo "<option value=\"$ge_id\">$ge_name</option>";
							}
							?>
						</select>
					</div>

				</div>

				<div class="row mt-3">
					<div class="col-md-3">
						<label>Date</label>
						<input type="text" id="date" class="form-control"/>
					</div>
					<div class="col-md-3">
						<label>Exchange Rate</label>
						<input type="text" id="exchange_rate" class="form-control"/>
					</div>
					<div class="col-md-3">
						<label>Deemed Energy</label>
						<input type="text" id="deemed_energy" class="form-control"/>
					</div>
				</div>

				<div class="row mt-3">
					<div class="col-md-3">
						<label>Quantity</label>
						<input type="text" id="qty" class="form-control"/>
					</div>
					<div class="col-md-3">
						<label>VAT</label>
						<input type="text" id="vat" class="form-control"/>
					</div>
					<div class="col-md-3">
						<label>Amount</label>
						<input type="text" id="amount" class="form-control"/>
					</div>
					<div class="col-md-3">
						<label>Total</label>
						<input type="text" id="total" class="form-control"/>
					</div>
				</div>

				<div class="row mt-4"><br/>
					<div class="col-lg-2">
						<div class="form-group">
							<button type="submit" name="submit" class="btn btn-primary" id="saveBtn">
								<i class="fa fa-fw fa-save"></i> Save
							</button>
						</div>
					</div>
				</div>
			</div>
		</form>

		<script>
			$(document).ready(function() {
				$('#saveBtn').on('click', function(e) {
					e.preventDefault();

					// Basic validation
					let errors = [];
					const fdn = $('#fdn').val().trim();
					const transaction_type = $('#transaction_type').val();
					const year = $('#year').val();
					const month = $('#month').val();
					const generator = $('#generator').val();
					const date = $('#date').val().trim();
					const exchange_rate = $('#exchange_rate').val().trim();
					const deemed_energy = $('#deemed_energy').val().trim();
					const qty = $('#qty').val().trim();
					const vat = $('#vat').val().trim();
					const amount = $('#amount').val().trim();
					const total = $('#total').val().trim();

					// Required fields check
					if (!fdn) errors.push('FDN is required.');
					if (!transaction_type) errors.push('Transaction type is required.');
					if (!year || year === 'Select') errors.push('Year is required.');
					if (!month || month === 'Select') errors.push('Month is required.');
					if (!generator || generator === 'Select') errors.push('Generator is required.');
					if (!date) errors.push('Date is required.');
					if (!exchange_rate || isNaN(exchange_rate)) errors.push('Exchange rate must be a number.');
					if (!deemed_energy || isNaN(deemed_energy)) errors.push('Deemed energy must be a number.');
					if (!qty || isNaN(qty)) errors.push('Quantity must be a number.');
					if (!vat || isNaN(vat)) errors.push('VAT must be a number.');
					if (!amount || isNaN(amount)) errors.push('Amount must be a number.');
					if (!total || isNaN(total)) errors.push('Total must be a number.');

					if (errors.length > 0) {
						alert(errors.join('\n'));
						return;
					}

					// If valid, send AJAX
					$.ajax({
						url: "<?php echo return_url().'ajax/process_purchase.php'; ?>",
						type: 'POST',
						data: {
							fdn, transaction_type, year, month, generator, date,
							exchange_rate, deemed_energy, qty, vat, amount, total
						},
						success: function(response) {
							alert('Saved successfully: ' + response);
						},
						error: function(xhr, status, error) {
							alert('Error: ' + error);
						}
					});
				});
			});
			</script>

		<?php
	}


	public function bill(){

		$id = 2029;
		$year = portion(4);
		$month = portion(5);
		$note = portion(6);

		$year = 2020;
		$month = 8;

		?>
<form class="" action="" method="post" enctype="multipart/form-data">
			<?php
					
			if(isset($_POST['filter'])){
				$year_uploaded = $_POST['filter_year'];
				$month_uploaded = $_POST['filter_month'];

				FeedBack::redirect(return_url().portion(1).'/'.portion(2).'/'.$year_uploaded.'/'.$month_uploaded);
			}else{
				$year = strtotime('-1 month', time());

				$month_uploaded = (portion(4))? portion(4) : date('m', $year);
				$year_uploaded = (portion(3))? portion(3) : date('Y', $year);
			}
			
			?>
			
		</form>
		<form class="search" action="" method="post">
			<fieldset >
				<div class="pull-left"> Year: 
					<select name="filter_year" style="width:80px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=2019; $i<=date('Y'); $i++){
							if($i == date('Y') && empty($year_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							elseif($i == $year_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
							else
								echo '<option value="'.$i.'">'.($i).'</option>';
						}
						?>
					</select>
				</div>
				<div class="pull-left">
					Month: 
					<select name="filter_month" style="width:120px; margin: auto 10px;">
						<option>Select</option>
						<?php 
						for($i=1; $i<=12; $i++){
							if($i == date('m') && empty($month_uploaded))
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							elseif($i == $month_uploaded)
								echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
							else
								echo '<option value="'.$i.'">'.month_name($i).'</option>';
						}
						?>
					</select>
				</div>	
				<div  class="pull-left">
					<input type="submit" value="Show" name="filter" style="width:100%; margin: auto 10px;"/>
				</div>
				<div class="clear"></div>
			</fieldset>
		</form>
		<br/>

		<?php

		$year = $year_uploaded;
		$month = $month_uploaded;


		$db = new Db();
		$select = $db->select("SELECT * FROM customer WHERE customer_id = '$id'");
		extract($select[0][0]);
		
		
		//echo '<a href="'.return_url().'"><h4>Use Tanseco</h4></a>';
	
	?>
	<div id="side">
		<div class="">		
		<?php 
		
		echo '<div class="clearfix"></div>';

		echo '<h4>Readings in: <b>KWh</b>';
		echo '</h4>';

		if($this->isLocked($id, $year, $month)){
			$uu = " readonly ";
		}else{
			$uu = "";
		}
		
		if($id == 2029||$id == 2017){ // tanseco
			if(isset($_POST['saveValues'])){
				$price = $_POST['price'];
				$price2 = $_POST['price2'];
				$price3 = $_POST['price3'];
				$units = $_POST['units'];
				$generator = $_POST['generator'];
				$currency = $_POST['currency'];
				
				$shoulder = $_POST['shoulder'];
				$peak = $_POST['peak'];
				$offpeak = $_POST['offpeak']; 
				$deemed_unit = $_POST['deemed_unit'];
				$deemed_amount = $_POST['deemed_amount'];

				// print_r($price);
				// echo '<br/>';echo '<br/>';
				// print_r($price2);
				// echo '<br/>';echo '<br/>';
				// print_r($price3);

				$loss_factor = str_replace(',', '', $_POST['loss_factor']);
				$dollars = str_replace(',', '', $_POST['dollars']);
				$euros = str_replace(',', '', $_POST['euros']);

				$db = new Db();

				$sel = $db->select("SELECT ex_id FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month' ");

				if($db->num_rows()){
					extract($sel[0][0]);
					//echo '>>'.$ex_id.'<<';
					$update = $db->update("exchange_rate", [
						"ex_usd"=>$dollars,
						"ex_euro"=>$euros,
						"ex_loss_factor"=>$loss_factor,
						"ex_added_by"=>user_id(),
						"ex_date_added"=>time(),
						//"ex_month"=>$month,
						//"ex_year"=>$year,
					], ["ex_id"=>$ex_id]);

					echo $db->error();
				}else{
					$insert = $db->insert("exchange_rate", [
						"ex_usd"=>$dollars,
						"ex_euro"=>$euros,
						"ex_loss_factor"=>$loss_factor,
						"ex_added_by"=>user_id(),
						"ex_date_added"=>time(),
						"ex_month"=>$month,
						"ex_year"=>$year,
					]);
				}

				echo $db->error();
				
				for($i=0; $i<count($generator); $i++){

					$g = $generator[$i];
					$u = str_replace(',', '', $units[$i])*1000;
					$p = str_replace(',', '', $price[$i])*1000;
					$p2 = str_replace(',', '', $price2[$i])*1000;
					$p3 = str_replace(',', '', $price3[$i])*1000;
					$c = str_replace(',','',$currency[$i])*1000;
					
					$s = str_replace(',','',$shoulder[$i])*1000;
					$pp = str_replace(',','',$peak[$i])*1000;
					$o = str_replace(',','',$offpeak[$i])*1000;
					$de_unit = str_replace(',','',$deemed_unit[$i]);
					$de_amount = str_replace(',','',$deemed_amount[$i]);

					$select = $db->select("SELECT TOP 1 mix_formulae as formulae, mix_narration as narration FROM mix_price WHERE mix_generator_id = '$g' AND mix_formulae IS NOT NULL ORDER BY mix_id DESC");
					extract($select[0][0]);

					$select = $db->select("SELECT * FROM mix_price WHERE mix_generator_id = '$g' AND mix_year = '$year' AND mix_month = '$month'");
					
					if($db->num_rows()){
						extract($select[0][0]);
						$update = $db->update("mix_price", [
							"mix_generator_id"=>$g,
							"mix_units"=>$u,
							"mix_amount"=>$p,
							"mix_amount2"=>$p2,
							"mix_amount3"=>$p3,
							//"mix_date_added"=>time(),
							"mix_added_by"=>user_id(),
							"mix_month"=>$month,
							"mix_year"=>$year,
							"mix_currency"=>$c,
							"mix_shoulder"=>$s,
							"mix_peak"=>$pp,
							"mix_offpeak"=>$o, 
							"mix_deemed_unit"=>$de_unit,
							"mix_deemed_amount"=>$de_amount,
						], ["mix_id"=>$mix_id]);

						
							// if($c=='EUR'){
							// 	$cur = 104;
							// }else if($c=='USD'){
							// 	$cur = 102;
							// }else if($c=='UGX'){
							// 	$cur = 101;
							// }
							if($p > 0){
								$gross = $p;
								$cur = 102;
							}elseif($p2 > 0){
								$gross = $p2;
								$cur = 104;
							}elseif($p3 > 0){
								$gross = $p3;
								$cur = 101;
							}
							$d = new Db();
							$select = $d->select("SELECT * FROM other_invoice WHERE oi_date_added = '$mix_date_added' AND oi_source = 'PURCHASES' AND oi_customer_id='$g'");
							$ss = $d->num_rows();
					       if($ss){
					       
					       if($u !='0'){
					       	$db = new Db();
							$db->update("other_invoice",["oi_customer"=>$g,"oi_pay_template"=>1,"oi_currency"=>$cur,"oi_added_by"=>user_id(),"oi_date_added"=>$mix_date_added,"oi_date"=>$mix_date_added,"oi_ref_no"=>$mix_date_added, "oi_vat"=>0,"oi_fdn"=>0, "oi_vc"=>0, "oi_qr"=>0, "oi_invoice_id"=>$mix_date_added,"oi_source"=>'PURCHASES',"oi_exchange_rate"=>0,"oi_invoice_date"=>$mix_date_added,
								"oi_peak" =>$pp,
								"oi_shoulder" =>$s,
								"oi_off_peak" =>$o,
								"oi_payable" =>0,
								"oi_collectable" => 0,
								"oi_gross" => $gross,
								"oi_vat_amount" =>0,
								"oi_net" => 0,
								"oi_post_date"=>time(),
								"oi_status" => 1,
								"oi_counter" => 1],["oi_rel"=>$mix_date_added,"oi_customer_id"=>$g]);
						   }
						}else{
							$db = new Db();
							if($u !='0'){
							$db->insert("other_invoice",["oi_customer"=>$g,"oi_customer_id"=>$g,"oi_pay_template"=>1,"oi_currency"=>$cur,"oi_added_by"=>user_id(),"oi_date_added"=>$mix_date_added,"oi_date"=>$mix_date_added,"oi_rel"=>$mix_date_added,"oi_ref_no"=>$mix_date_added, "oi_vat"=>0,"oi_fdn"=>0, "oi_vc"=>0, "oi_qr"=>0, "oi_invoice_id"=>$mix_date_added,"oi_source"=>'PURCHASES',"oi_exchange_rate"=>0,"oi_invoice_date"=>$mix_date_added,
								"oi_peak" =>$pp,
								"oi_shoulder" =>$s,
								"oi_off_peak" =>$o,
								"oi_payable" =>0,
								"oi_collectable" => 0,
								"oi_gross" => $gross,
								"oi_vat_amount" =>0,
								"oi_net" => 0,
								"oi_post_date"=>$mix_date_added,
								"oi_status" => 1,
								"oi_counter" => 1]);
						     }
						}

					}else{
						$insert = $db->insert("mix_price", [
							"mix_generator_id"=>$g,
							"mix_units"=>$u,
							"mix_amount"=>$p,
							"mix_amount2"=>$p2,
							"mix_amount3"=>$p3,
							"mix_date_added"=>time(),
							"mix_added_by"=>user_id(),
							"mix_month"=>$month,
							"mix_year"=>$year,
							"mix_currency"=>$c,
							"mix_shoulder"=>$s,
							"mix_peak"=>$pp,
							"mix_offpeak"=>$o,
							"mix_deemed_unit"=>$de_unit,
							"mix_deemed_amount"=>$de_amount,
							"mix_formulae"=>$formulae,
							"mix_narration"=>$narration,
						 ]);
						// 	$db = new Db();
						// 	if($c=='EUR'){
						// 		$cur = 104;
						// 	}else if($c=='USD'){
						// 		$cur = 102;
						// 	}else if($c=='UGX'){
						// 		$cur = 101;
						// 	}
							if($p > 0){
								$gross = $p;
								$cur = 102;
							}elseif($p2 > 0){
								$gross = $p2;
								$cur = 104;
							}elseif($p3 > 0){
								$gross = $p3;
								$cur = 101;
							}
							if($u !='0'){
						$db->insert("other_invoice",["oi_customer"=>$g,"oi_customer_id"=>$g,"oi_pay_template"=>1,"oi_currency"=>$cur,"oi_added_by"=>user_id(),"oi_date_added"=>time(),"oi_date"=>time(),"oi_rel"=>time(),"oi_ref_no"=>time(), "oi_vat"=>0,"oi_fdn"=>0, "oi_vc"=>0, "oi_qr"=>0, "oi_invoice_id"=>time(),"oi_source"=>'PURCHASES',"oi_exchange_rate"=>0,"oi_invoice_date"=>time(),

							"oi_peak" =>$pp,
							"oi_shoulder" =>$s,
							"oi_off_peak" =>$o,
							"oi_payable" =>0,
							"oi_collectable" => 0,
							"oi_gross" => $gross,
							"oi_vat_amount" =>0,
							"oi_net" => 0,
							"oi_post_date"=>time(),
							"oi_status" => 1,
							"oi_counter" => 1]);
					   }
					}

					if($db->error()){
						Feedback::error($db->error());
					}
				}
			}
			//echo "SELECT * FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'";
			$select = $db->select("SELECT * FROM exchange_rate WHERE ex_year = '$year' AND ex_month = '$month'");
			extract($select[0][0]);
			//print_r($select[0][0]);

			if(empty($dollars)){
				$ex_usd = number_format($ex_usd, 5);
				$dollars = ($ex_usd);
			}
			if(empty($euro)){
				$ex_euro = number_format($ex_euro, 5);
				$euros = ($ex_euro);
			}
			if(empty($loss_factor)){
				$ex_loss_factor = number_format($ex_loss_factor, 5);
				$loss_factor = ($ex_loss_factor);
			}
			echo '<form action="" method="post">';
			//echo '<h4>Exchange Rate Applied</h4>';
			echo '<br/>';
			echo '<div style="padding-right:40px; float:left;"><b>Dollars</b></b>: <input style="width:100px;" type="text"  value="'.$dollars.'" name="dollars" class="number"/></div>';
			echo '<div style="padding-right:40px; float:left;"><b>Euros</b>: <input style="width:100px;" type="text" value="'.$euros.'"  name="euros" class="number"/></div>';
			echo '<div style="padding-right:40px; float:left;">Transmission Loss Factor <b>(Ref ERA)</b>: <input type="text" value="'.$loss_factor.'"  name="loss_factor" class="number" style="width:100px;"/></div>';
			echo '<div style="padding-right:40px; float:left;"><input type="submit" name="saveValues" value="Save"/></div>';


			echo '<style>table>input{ background-color: #fff; width:100% !important; border:none;}</style>';

			echo '<div style="clear:both;"><br/></div>';
			echo '<h4>Generators</h4>';

			$rt = 'background:#d7d7f7 !important;';
			$rt = 'background:#f5e8ec !important; border:1px solid #222;';

			echo '<table id="table" border="1" cellpadding="2" style="width:100%;">';
			
			echo '<tr style="background-color:white;">';
			echo '<th class="text-center" style="margin-top:180px; width:30px;">No.</th>';
			echo '<th class="text-center" style="">Customer Name</th>';
			echo '<th class="text-center" style="width:40px;">Bill Units<br/>(kWh)</th>';
			echo '<th class="text-center" style="width:40px;">Bill Amount(USD)</th>';
			echo '<th class="text-center" style="width:40px;">Bill Amount(EUR)</th>';
			echo '<th class="text-center" style="width:40px;">Bill Amount(UGX)</th>';
			echo '<th class="text-center" style="width:40px;">Shoulder</th>';
			echo '<th class="text-center" style="width:40px;">Peak</th>';
			echo '<th class="text-center" style="width:40px;">Off Peak</th>';
			echo '<th class="text-center" style="width:40px;">Deemed Energy<br/> Units</th>';
			echo '<th class="text-center" style="width:40px;">Deemed Energy<br/>Amount</th>';
			echo '</tr>';
			

			$no=1;
			$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC ");
			foreach($select[0] as $row){
				extract($row);

				$sel = $db->select("SELECT * FROM mix_price WHERE mix_generator_id = '$ge_id' AND mix_year = '$year' AND mix_month = '$month'");
				extract($sel[0][0]);

				if($units[$i]){
					$mix_units = ($units[$i]);
				}
				if($price[$i]){
					$mix_amount = ($price[$i]);
				}

				if($price2[$i]){
					$mix_amount2 = ($price2[$i]);
				}

				if($price3[$i]){
					$mix_amount3 = ($price3[$i]);
				}

				if($shoulder[$i]){
					$mix_shoulder = ($shoulder[$i]);
				}

				if($peak[$i]){
					$mix_peak = ($peak[$i]);
				}

				if($offpeak[$i]){
					$mix_offpeak = ($offpeak[$i]);
				}

				$mix_units /= (pow(1000,1));
				$mix_amount /= (pow(1000,1));
				$mix_amount2 /= (pow(1000,1));
				$mix_amount3 /= (pow(1000,1));
				$mix_shoulder /= (pow(1000,1));
				$mix_peak /= (pow(1000,1));
				$mix_offpeak /= (pow(1000,1));

				$mix_units = $this->decimalFixer($mix_units);
				$mix_amount = $this->decimalFixer($mix_amount);
				$mix_amount2 = $this->decimalFixer($mix_amount2);
				$mix_amount3 = $this->decimalFixer($mix_amount3);
				$mix_shoulder = $this->decimalFixer($mix_shoulder);
				$mix_peak = $this->decimalFixer($mix_peak);
				$mix_offpeak = $this->decimalFixer($mix_offpeak);
				
				echo '<input type="hidden" value="'.$ge_id.'" name="generator[]"/>';
				echo '<input type="hidden" value="'.$ge_currency.'" name="currency[]"/>';

				echo '<tr>';
				echo '<td>'.($no++).'.</td>';
				echo '<td>'.$ge_name.'</td>';
				echo '<td><input style="width:110px;" type="text" value="'.$mix_units.'" autocomplete="off" name="units[]" class="number text-right"/></td>';

				if($ge_id == 4){
					$u = " type='text'  placeholder='In USD' ";
					$e = " type='hidden' ";
					$x = " type='text'   placeholder='In UGX' ";
				}elseif($ge_id == 10){
					$u = " type='text'  placeholder='In USD' ";
					$e = " type='text'  placeholder='In UGX' ";
					$x = " type='hidden' ";
				}elseif($ge_currency == "UGX"){
					$u = " type='hidden'  placeholder='In USD' ";
					$e = " type='hidden'  placeholder='In EUR' ";
					$x = " type='text'  placeholder='In UGX' ";
				}elseif($ge_currency == "EUR"){
					$u = " type='hidden'  placeholder='In USD' ";
					$e = " type='text'  placeholder='In EUR' ";
					$x = " type='hidden'  placeholder='In UGX' ";
				}elseif($ge_currency == "USD"){
					$u = " type='text'  placeholder='In USD' ";
					$e = " type='hidden'  placeholder='In EUR' ";
					$x = " type='hidden'  placeholder='In UGX' ";
				}else{
					$u = ' type="text" placeholder="In USD" ';
					$e = ' type="hidden" ';
					$x = ' type="hidden" ';
				}

				echo '<td><input style="width:110px;" '.$u.' value="'.$mix_amount.'" autocomplete="off" name="price[]" class="number text-right"/></td>';

				echo '<td><input style="width:110px;" '.$e.' value="'.$mix_amount2.'" autocomplete="off" name="price2[]" class="number text-right"/></td>';

				echo '<td><input style="width:110px;" '.$x.' value="'.$mix_amount3.'" autocomplete="off" name="price3[]" class="number text-right"/></td>';

				echo '<td><input style="width:110px; '.$rt.'" value="'.$mix_shoulder.'" autocomplete="off" name="shoulder[]" class="number text-right"/></td>';
				echo '<td><input style="width:110px; '.$rt.'" value="'.$mix_peak.'" autocomplete="off" name="peak[]" class="number text-right"/></td>';
				echo '<td><input style="width:100px; '.$rt.'" value="'.$mix_offpeak.'" autocomplete="off" name="offpeak[]" class="number text-right"/></td>';
				echo '<td><input style="width:100px; '.$rt.'" value="'.$mix_deemed_unit.'" autocomplete="off" name="deemed_unit[]" class="number text-right"/></td>';
				echo '<td><input style="width:100px; '.$rt.'" value="'.$mix_deemed_amount.'" autocomplete="off" name="deemed_amount[]" class="number text-right"/></td>';

				echo '</tr>';
			}

			echo '</table>';

			echo '</form>';
		}
	}

	

}