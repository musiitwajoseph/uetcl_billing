<?php
Class Customers extends BeforeAndAfter{
	public $page = "CUSTOMERS";
	
	public $set_class = "";
	public $set_method = "";
	public $set_customer = "";
	public $set_year = "";
	public $set_month = "";

	public function __construct(){
		$access = new AccessRights();
		$access->pageAccess(user_id(), $this->page, 'V');
	}

	public function parameters($href){
		$portion = explode('/',str_replace(return_url(), '', $href));

		$this->set_class = $portion[0];
		$this->set_method = $portion[1];
		$this->set_customer = $portion[2];
		$this->set_year = $portion[3];
		$this->set_month = $portion[4];
	}
	
	public function getLinks(){
		$page = "CUSTOMERS";
		$links = array(
			array(
				"link_name"=>"Add Customer", 
				"link_address"=>"customers/add-customer",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"Customer List", 
				"link_address"=>"customers/all-customers",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}
	
	public function addCustomerAction(){

		//echo date('Y-m-d', 1681509600);
		if(isset($_POST['submit'])){
			
			$short_name = $_POST['short_name'];
			$full_name = $_POST['full_name'];
			$address = $_POST['address'];
			$default_units = $_POST['default_units'];
			$wheeling_charge = $_POST['wheeling_charge'];
			$currency = $_POST['currency'];
			$accounts_due = $_POST['accounts_due'];
			$peak = $_POST['peak'];
			$off_peak = $_POST['off_peak'];
			$shoulder = $_POST['shoulder'];
			$vat = $_POST['vat'];
			$journal_no = $_POST['journal_no'];

			$postal_code = $_POST['postal_code'];
			$street = $_POST['street'];
			$email = $_POST['email'];
			$telephone = $_POST['telephone'];
			$address = trim($_POST['address']);
			$tin_number = $_POST['tin_number'];
			$cust_type = trim($_POST['cust_type']);
			if(empty($postal_code)){
				$errors[]="Enter Postal Code and District";
			}
			if(empty($street)){
				$errors[]="Enter Plot no. and Street";
			}
			if(empty($email)){
				$errors[]="Enter Email";
			}
			if(empty($telephone)){
				$errors[]="Enter Telephone";
			}

			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();

			if(empty($short_name)){
				$errors[]="Enter Short Name";
			}
			if(empty($full_name)){
				$errors[]="Enter Full Name";
			}
			// if(empty($address)){
			// 	$errors[]="Enter Address";
			// }
			// if(empty($default_units)){
			// 	$errors[]="Enter Default Units";
			// }
			// if(($wheeling_charge=="")){
			// 	$errors[]="Enter Wheeling Charge";
			// }
			if(($currency=="")){
				$errors[]="Select Currency";
			}
			// if(empty($peak)){
			// 	$errors[]="Enter Peak Rate";
			// }
			if(empty($accounts_due)){
				$errors[]="Enter Accounts Due Statement";
			}
			// if(empty($off_peak)){
			// 	$errors[]="Enter Off Peak Rate";
			// }
			// if(empty($shoulder)){
			// 	$errors[]="Enter Shoulder Rate";
			// }
			if(empty($cust_type)){
				$errors[]="Select Customer Type";
			}
			// if(empty($tin_number)){
			// 	$errors[]="Enter Shoulder Rate";
			// }
			if($this->isThere("customer", ["customer_full_name"=>$full_name])){
				$errors[]="Customer Name($full_name) already exists";
			}
			if($this->isThere("customer", ["customer_short_name"=>$short_name])){
				$errors[]="Customer Short Name($short_name) already exists";
			}


			$image = new Image();
			$image->upload("file");
			$agreement = $image->imageLink();			

			if($image->errorMessage() != "Uploaded" && !empty($image->errorMessage())){
				$errors[] = $image->errorMessage();
			}		


			if(empty($errors)){

				$db = new Db();
				$x = $db->insert("customer",
					[
						"customer_full_name"=>$full_name, 
						"customer_short_name"=>$short_name,
						"customer_journal_no"=>$journal_no,
						"customer_address"=>$address, 
						"customer_default_units"=>$default_units, 
						"customer_added_by"=>user_id(), 
						"customer_date_added"=>time(), 
						"customer_vat"=>$vat, 
						"customer_currency"=>$currency,
						"customer_accounts_due"=>$accounts_due,
						"customer_street"=>$street,
						"customer_postal_code"=>$postal_code,
						"customer_tel"=>$telephone,
						"customer_email"=>$email,
						"customer_agreement"=>$agreement,
						"customer_tin"=>$tin_number,
						"customer_type"=>$cust_type,
					]);
				//FeedBack::errors($db->error());
				//customer id
				$db1 =  new Db();
				$select = $db1->select("SELECT customer_id FROM customer ORDER BY customer_id DESC");
				extract($select[0][0]);

				$insert = $db->insert("wheeling_charge",[
					"wc_rate"=>$wheeling_charge,
					"wc_customer_id"=>$customer_id,
					"wc_date_added"=>time(),
					"wc_added_by"=>user_id()
				]);

				$insert = $db->insert("time_band",[
					"tb_peak"=>$peak,
					"tb_customer_id"=>$customer_id,
					"tb_off_peak"=>$off_peak,
					"tb_shoulder"=>$shoulder,
					"tb_added_by"=>user_id(),
					"tb_date_added"=>time()
				]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/all-customers');
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
					Add Energy Customer Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" enctype="multipart/form-data" method="post">
								<div class="row">
									<div class="col-md-3">
										<div class="form-group">
											<label>Short Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="short_name" placeholder="" value="<?php echo @$short_name; ?>">
											</div>
										</div>
									</div>
									<div class="col-md-9">
										<div class="form-group">
											<label>Full Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="full_name" placeholder="" value="<?php echo @$full_name; ?>">
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<div class="col-lg-12">
										<div class="form-group">
											<label>Address<span class="must">*</span></label>
											<div  class="col-lg-12">
												<div class="form-line">
													<label>Plot No. & Street</label>
													<textarea required style="height:50px;"class="form-control" name="street"><?php echo $street; ?></textarea>
												</div>
												<div class="form-line">
													<label>Postal code & District</label>
													<textarea style="height:50px;"class="form-control" name="postal_code"><?php echo $postal_code; ?></textarea>
												</div>
												<div class="row">
													<div class="col-md-3">
														<div class="form-line">
															<label>Telphone</label>
															<textarea style="height:50px;"class="form-control" name="telephone"><?php echo $telephone; ?></textarea>
														</div>
													</div>
													<div class="col-md-6">
														<div class="form-line">
															<label>Email</label>
															<textarea style="height:50px;"class="form-control" name="email"><?php echo $email; ?></textarea>
														</div>
													</div>
													<div class="col-md-3">
														<div class="form-line">
															<label>GL Account</label>
															<textarea style="height:50px;"class="form-control" name="journal_no"><?php echo $journal_no; ?></textarea>
														</div>
													</div>
												</div>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<br/>
									<div class="col-lg-12">
										<div class="row">											
											<div class="col-lg-3">
												<div class="form-group">
													<?php 
													if($vat == ""){
														$vat = 18;
													}
													?>
													<label>VAT (%)<span class="must">*</span></label>
													<div class="form-line">
														<input type="number" min="0" step="any" value="<?php echo $vat; ?>" class="form-control" name="vat">
													</div>
												</div>
											</div>											
											<div class="col-lg-3">
												<div class="form-group">
													<label>Currency<span class="must">*</span></label>
													<div class="form-line">
														<select name="currency">
															<option value="">--- Select ---</option>
															<?php
															$s = new Db();
															$ss = $s->select("SELECT * FROM currency");
															foreach($ss[0]as $s){
																extract($s);

																if($customer_currency == $currency_id)
																	echo '<option selected="selected" value="'.$currency_id.'">'.$currency_name.' ('.$currency_symbol.')</option>';
																else
																	echo '<option value="'.$currency_id.'">'.$currency_name.' ('.$currency_symbol.')</option>';

															}
															?>
															
														</select>
													</div>
												</div>
											</div>											
											<div class="col-md-6">
												<div class="form-group">
													<label>Accounts Due Statement<span class="must">*</span></label>
													<div class="form-line">
														<?php if(empty($accounts_due)){ $accounts_due = "Accounts are due within 45 days of receipt of invoice";}
														?>
														<input class="form-control" type="text" name="accounts_due" placeholder="" value="<?php echo @$accounts_due; ?>">
													</div>
												</div>
											</div>
										</div>
									</div>
									<div class="col-lg-2">
										<label>Ruling Rates:<span class="must"></span></label>
									</div>
									<div class="col-lg-12">
										<div class="row">											
											<div class="col-lg-2" style="display: none;">
												<div class="form-group">
													<label>Peak<span class="must">*</span></label>
													<div class="form-line">
														<input type="hidden" min="0" step="any" value="0" class="form-control" name="peak">
													</div>
												</div>
											</div>
											<div class="col-lg-2" style="display: none;">
												<div class="form-group">
													<label>Shoulder<span class="must">*</span></label>
													<div class="form-line">
														<input type="hidden" min="0" step="any" value="0" class="form-control" name="shoulder">
													</div>
												</div>
											</div>
											<div class="col-lg-2" style="display: none;">
												<div class="form-group">
													<label>Off Peak<span class="must">*</span></label>
													<div class="form-line">
														<input type="hidden" min="0" step="any" value="0" class="form-control" name="off_peak">
													</div>
												</div>
											</div>
											 <div class="col-md-3">
												<label>TIN<span class="must"></span></label>
												<div class="form-line">
												  <input type="text" name="tin_number" value="<?php echo$tin_number;?>" class="form-control" placeholder="TIN">
											   </div>
											  </div>
											  <div class="col-md-3">
												<label>Customer Type<span class="must"></span></label>
												<div class="form-line">
												  <select class="form-control" name="cust_type">
												  	<option value="B2B">B2B</option>
												  	<option value="B2G">B2G</option>
												  	<option value="B2C">B2C</option>
												  	<option value="B2F">B2F</option>
												  </select>
											   </div>
											  </div>
										</div>
									</div>									
									<div class="col-md-9">
										<div class="form-group">
											<label>Attach agreement<span class="must">*</span> <span class="text-muted" style="font-size: 12px;"> Only Pdf documents are recommended</span></label>
											<div class="form-line">
												<input class="form-control" type="file" name="file" placeholder="" value="">
											</div>
										</div>
									</div>
									<div class="col-lg-12">
										<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}
	public function addBoundaryNodeAction(){
		if(isset($_POST['submit'])){
			
			$metering_point = $_POST['metering_point'];
			$customer_payable_to = $_POST['customer_payable_to'];
			$consider = $_POST['consider'];

			$time = time();
			$user = user_id();

			$db = new Db();
			
			$errors = array();

			if(empty($metering_point)){
				$errors[]="Select Metering Point";
			}
			if(empty($customer_payable_to)){
				$errors[]="Select Customer Payable To";
			}
			if(empty($consider)){
				$errors[]="consider";
			}

			if(empty($errors)){
				$db = new Db();
				$cus = $this->rgf("metering_point", $metering_point, "mp_id", "mp_customer_id");
				$x = $db->insert("boundary_node",["bn_customer"=>$cus, "bn_metering_point"=>$metering_point, "bn_customer_payable_to"=>$customer_payable_to,"bn_consider"=>$consider]);
				//FeedBack::errors($db->error());
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/add-boundary-node');
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
					Add Boundary Form
				</div>
				<div class="panel-body">
					
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-md-3">
										<div class="form-group">
											<label>Metering Point<span class="must">*</span></label>
											<div class="form-line">
												<select name="metering_point">
													<option value=""> Select</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM metering_point ORDER BY mp_location ASC");

													foreach($select[0] as $row){
														extract($row);

														if(!$this->isThere("boundary_node", ["bn_metering_point"=>$mp_id])){		
															echo '<option value="'.$mp_id.'">'.$mp_location.'</option>';
														}
													}
													?>
												</select>
											</div>
										</div>
										<!-- Collectable from Metering Point -->
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Payable to<span class="must">*</span></label>
											<div class="form-line">
												<select name="customer_payable_to">
													<option value=""> Select</option>
													<?php
													$db = new Db();

													$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");

													foreach($select[0] as $row){
														extract($row);

														echo '<option value="'.$customer_id.'">'.$customer_short_name.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Consider<span class="must">*</span></label>
											<div class="form-line">
												<select name="consider">
													<option value="1">Imports</option>
													<option value="2">Exports</option>
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
					<tr>
						<th width="40px">No.</th>
						<th>Customer<br/>Collectable from</th>
						<th>Metering Point<br/>Collectable from</th>
						<th>Customer<br/>Payable To</th>
						<th>Consider</th>
						<th width="100px">Action</th>
					</tr>
				</thead>
				<tbody>
				<?php 
				$db = new Db();
				$select = $db->select("SELECT * FROM boundary_node");
				$g = 0;
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
						echo '<td>'.(++$g).'.</td>';
						echo '<td>'.strtoupper($this->rgf("customer", $bn_customer, "customer_id", "customer_short_name")).'</td>';
						echo '<td>'.strtoupper($this->rgf("metering_point", $bn_metering_point, "mp_id", "mp_location")).'</td>';
						echo '<td>'.$this->rgf("customer", $bn_customer_payable_to,"customer_id", "customer_short_name") .'</td>';
						echo '<td>'.$this->consider($bn_consider).'</td>';
						echo '<td><a href="'.return_url().'customers/delete-boundary-node/'.$bn_id.'"><i class="fa fa-fw fa-times"></i>&nbsp;Delete</a></td>';						
					echo '</tr>';
				}

				?>
				</tbody>

			</table>
		</div>
	<?php
	}

	
	public function addGeneratorOld(){
		echo '<h4>ADD GENERATOR</h4>';
		if(isset($_POST['submit'])){
			
			$metering_point = $_POST['metering_point'];
			$generator_category = $_POST['generator_category'];

			$time = time();
			$user = user_id();

			$db = new Db();
			
			$errors = array();

			if(empty($metering_point)){
				$errors[]="Select Metering Point";
			}
			if(empty($generator_category)){
				$errors[]="Select Generator Gategory";
			}
		

			if(empty($errors)){
				$db = new Db();

				$x = $db->insert("generator",["ge_metering_point"=>$metering_point, "ge_category"=>$generator_category]);
				//FeedBack::errors($db->error());
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/add-generator');
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
											<label>Metering Point<span class="must">*</span></label>
											<div class="form-line">
												<select name="metering_point">
													<option value=""> Select</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM metering_point ORDER BY mp_location ASC");

													foreach($select[0] as $row){
														extract($row);

														if(!$this->isThere("generator", ["ge_metering_point"=>$mp_id])){		
															echo '<option value="'.$mp_id.'">'.$mp_location.'</option>';
														}
													}
													?>
												</select>
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
						<th>Customer<br/>Collectable from</th>
						<th>Metering Point<br/>Collectable from</th>
						<th>Generator Category</th>
						<th>Action</th>
					</tr>
				</thead>
				<tbody>
				<?php 
				$db = new Db();
				$select = $db->select("SELECT * FROM generator ORDER BY ge_category ASC");
				$g = 0;
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
						echo '<td>'.(++$g).'.</td>';
						echo '<td>'.strtoupper($this->rgf("customer", $this->rgf("metering_point", $ge_metering_point, "mp_id", "mp_customer_id"), "customer_id", "customer_short_name")).'</td>';
						echo '<td>'.strtoupper($this->rgf("metering_point", $ge_metering_point, "mp_id", "mp_location")).'</td>';
						echo '<td>'.strtoupper($this->rgf("generator_category", $ge_category, "gc_id", "gc_name")).'</td>';
						
						echo '<td><a href="'.return_url().'customers/delete-generator/'.$ge_id.'"><i class="fa fa-fw fa-times"></i>&nbsp;Delete</a></td>';						
					echo '</tr>';
				}

				?>
				</tbody>

			</table>
		</div>
	<?php
	}




	public function deleteBoundaryNodeAction(){
		$id = portion(3);
		$db = new Db();
		$sql = "DELETE FROM boundary_node WHERE bn_id = '$id'";
		$delete = $db->query($sql);
		
		FeedBack::redirect(return_url().'customers/add-boundary-node');
	}

		public function deleteGLAccountAction(){
		$id = portion(3);
		$id2 = portion(4);
		$db = new Db();
		$sql = "DELETE FROM gl_account WHERE gl_id = '$id2'";
		$delete = $db->query($sql);
		FeedBack::redirect(return_url().'customers/energy-customer-details/'.$id);
	}

	public function consider($id){
		if($id == 1) return "Imports";
		return "Exports";
	}
	
	public function addMeteringPointAction(){

		$id = ($this->customer)?$this->customer:portion(3);
		echo $this->backToCustomerDetails($id);
		$customer_id = $id;
		//echo "@@@".$customer_id;
		if(isset($_POST['submit'])){
			
			$location = strtoupper($_POST['location']);
			$main_meter = $_POST['main_meter'];
			$check_meter = $_POST['check_meter'];
			//$wheeling_charge = $_POST['wheeling_charge'];
			//$advance = $_POST['advance'];
			$region_id = $_POST['region_id'];
			$time = time();
			$user = user_id();

			$db = new Db();
			
			$errors = array(); 

			if(empty($location)){
				$errors[]="Enter Location Name";
			}

			if(empty($region_id)){
				$errors[]="Select Metering Point Region";
			}

			//if($this->isThere("metering_point",["mp_customer_id"=>$id, "mp_location"=>$location])){
			if($this->isThere("metering_point",["mp_location"=>$location])){
				$errors[] = $location." Already exits";
			}

			if(empty($errors)){
				if($customer_id =='2040'){
					$db = new Db();
				$x = $db->insert("metering_point",["mp_customer_id"=>$id,"mp_location"=>$location, 
				"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id]);

				$x = $db->insert("metering_point",["mp_customer_id"=>2041,"mp_location"=>$location, 
				"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id]);
				}else{
				$db = new Db();
				$x = $db->insert("metering_point",["mp_customer_id"=>$id,"mp_location"=>$location, 
				"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id]);
				}
				
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/energy-customer-details/'.$id);
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}

		if(isset($_POST['upload'])){
			
			$metering_points = array('Region', 'Metering Point', 'Main Meter', 'Check Meter', 'Wheeling Charge', 'Advance');
			$advanceValues = array("Imports", "Exports");

			$error = array();		
			$filename=$_FILES["file"]["tmp_name"];

			if($_FILES["file"]["size"] > 0){					
				$file = fopen($filename, "r");
				$count = 0;
				$error = array();
				while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
					$count++;
					
					$col_1 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[0]))));
					$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));
					$col_3 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[2]))));
					$col_4 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[3]))));
					$col_5 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[4]))));
					$col_6 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[5]))));		

					if($count == 1){
						for($i=1; $i<=6; $i++){
							if(${"col_".$i} != strtoupper($metering_points[$i-1])){
								$error[] = '<b>Check Line Column B'.$i.':</b> Invalid Template Name <b>'.${"col_".$i}.'</b>';
							}
						}
					}

					if($count == 2) break;					
				}
				

				//check if metering point exist
				$region_ids = array();
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;
					$error = array();
					
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;						
						$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));

						if($count >= 2){
							$mepo[] = $col_2;
							$db = new Db();
							$select = $db->select("SELECT region_id FROM meter_region WHERE region_name = '$col_1'");

							if($db->num_rows()==0){
								$error[] = '<b>Check Line '.$count.':</b> Region <b>('.$col_1.')</b> does not exist in the System.';
							}else{
								extract($select[0][0]);
								$region_ids[] = $region_id;
							}
						}
					}
				}

				//check if advance exist
				$region_ids = array();
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;
					$error = array();
					
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;						
						$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));

						if($count >= 2){
							$mepo[] = $col_2;
							$db = new Db();
							$select = $db->select("SELECT region_id FROM meter_region WHERE region_name = '$col_1'");

							if($db->num_rows()==0){
								$error[] = '<b>Check Line '.$count.':</b> Region <b>('.$col_1.')</b> does not exist in the System.';
							}else{
								extract($select[0][0]);
								$region_ids[] = $region_id;
							}
						}
					}
				}

				//check if metering point exist
				if(empty($error)){
					$file = fopen($filename, "r");
					$count = 0;
					$error = array();
					
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;						
						$col_2 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[1]))));

						if($count >= 2){
							$mepo[] = $col_2;
							$db = new Db();
							$select = $db->select("SELECT mp_id FROM metering_point WHERE mp_customer_id = '$customer_id' AND mp_location = '$col_2'");

							if($db->num_rows() >= 1){
								$error[] = '<b>Check Line '.$count.':</b> Metering Point <b>('.$col_2.')</b> already exists in the System.';
							}
						}
					}
				}
				
				//upload final
				$count = 0;
				if(empty($error)){				
					global $con;

					$time = time();
					$user_id = user_id();

					$file = fopen($filename, "r");
					while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
						$count++;	

						$point = (trim(str_replace("'","\'",strip_tags($emapData[1]))));
						$rregionn = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[0]))));

						$col_3 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[2]))));
						$col_4 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[3]))));
						$col_5 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[4]))));
						$col_6 = @strtoupper(trim(str_replace("'","\'",strip_tags($emapData[5]))));

						$db = new Db();
						$select = $db->select("SELECT region_id FROM meter_region WHERE region_name = '$rregionn'");
						extract($select[0][0]);

						if(strtolower($col_6) == "exports"){
							$advance = 2;
						}else{
							$advance = 1;
						}

						$wheeling_charge = $col_5;
						$check_meter = $col_3;
						$main_meter = $col_4;
						
						if($count >= 2){
							$db = new Db();
							if(empty($errors)){
								$db = new Db();
								$x = $db->insert("metering_point",["mp_customer_id"=>$customer_id,"mp_location"=>$point, 
								"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_wheeling_charge"=>$wheeling_charge, "mp_advance"=>$advance, "mp_region_id"=>$region_id]);						
							}else{
								FeedBack::errors($errors);
							}
						}

					}

				}
				
			}else{
				$error[] = "Attach a CSV File.";
			}

			if(!empty($error)){
				FeedBack::errors($error);
			}else{
				FeedBack::refresh();
				FeedBack::success();
			}

		}
		?>
		<div class="col-md-12">
			<br/>
			<?php
			echo '<div class="col-lg-12">';
			echo '<form class="search" role="form" action="" enctype="multipart/form-data" method="post">';
			echo '<h5>Upload Metering Points for '.$this->rgf("customer", $id, "customer_id", "customer_full_name").'</h5>';
			echo '<div class="pull-left">';
			echo '<input type="file" name="file">';
			echo '</div>';

			echo '<div class="pull-left">';
			echo '<button type="sumbit" name="upload" class="btn btn-primary btn-xs"> <i class="fa fa-fw fa-upload"></i> Upload </button><br/><br/>';
			echo '</div>';
			echo '<div class="clearfix"></div>';
			echo '</form>';
			echo '<div class="clearfix"></div>';
			echo '</div><br/><br/>';
			?>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Metering Point for <b><?php echo $this->rgf("customer", $id, "customer_id", "customer_full_name"); ?></b>  Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" enctype="multipart/form-data" method="post">
								<br/>
								<div class="row">
									<div class="col-md-3">
										<div class="form-group">
											
											<label>Region<span class="must">*</span></label>
											<div class="form-line">
												<select name="region_id">
													<option value=""> Select </option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM meter_region");
													foreach($select[0] as $row){
														extract($row);
														if($region_id == $mp_region_id)
															echo '<option selected="selected" value="'.$region_id.'">'.$region_name.'</option>';
														else
															echo '<option value="'.$region_id.'">'.$region_name.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											
											<label>Location<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="location" placeholder="" value="<?php echo @$location; ?>">
											</div>
										</div>
									</div>								
									<div class="col-lg-2">
										<div class="form-group">										
											<label>Main Meter<span class="must"></span></label>
											<div class="form-line">
												<input type="text" value="<?php echo $main_meter; ?>" class="form-control" name="main_meter">
											</div>
										</div>
									</div>
									<div class="col-lg-2">
										<div class="form-group">
											<label>Check Meter<span class="must"></span></label>
											<div class="form-line">
												<input type="text" step="" value="<?php echo $check_meter; ?>" class="form-control" name="check_meter">
											</div>
										</div>
									</div>										
									
									<div class="col-lg-12">
										<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}

	public function backToCustomerDetails($customer_id){
		return '<a class="eagle-load" href="'.return_url().'customers/energy-customer-details/'.$customer_id.'"><i class="fa fa-fw fa-arrow-left"></i> Back</a><div style="margin-bottom:20px;"></div>';
	}

	public function editMeteringPointAction(){
		$id = portion(3);
		$id2 = portion(4);

		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point WHERE mp_id = '$id2'");
		extract($select[0][0]);

		$main_meter = $mp_main_meter;
		$check_meter = $mp_check_meter;
		$location = $mp_location;

		if(isset($_POST['submit'])){
			
			$oldname = strtoupper($_POST['oldname']);
			$location = strtoupper($_POST['location']);
			$main_meter = $_POST['main_meter'];
			$check_meter = $_POST['check_meter'];
			$region_id = $_POST['region_id'];
			$time = time();
			$user = user_id();

			$db = new Db();
			
			$errors = array();

			if(empty($location)){
				$errors[]="Enter Location Name";
			}
			if(empty($region_id)){
				$errors[]="Select Metering Point Region";
			}

			if(empty($errors)){
				
				if($id == '2040'){
			     /////UMEME		
				$db = new Db();
				$x = $db->update("metering_point",["mp_location"=>$location, 
				"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id],["mp_id"=>$id2]);
					
				/////backflows 
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id='2041' AND mp_location='$oldname'");
				if($db->num_rows()){
					extract($select[0][0]);
					$db = new Db();
					$x = $db->update("metering_point",["mp_location"=>$location, 
					"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id],["mp_id"=>$mp_id]);
				    }
				}elseif($id == '2041'){
			     /////UMEME		
				$db = new Db();
				$x = $db->update("metering_point",["mp_location"=>$location, 
				"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id],["mp_id"=>$id2]);
					
				/////backflows 
				$db = new Db();
				$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id='2040' AND mp_location='$oldname'");
				if($db->num_rows()){
					extract($select[0][0]);
					$db = new Db();
					$x = $db->update("metering_point",["mp_location"=>$location, 
					"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id],["mp_id"=>$mp_id]);
				    }
				}
				else{
				$db = new Db();
				$x = $db->update("metering_point",["mp_location"=>$location, 
				"mp_main_meter"=>$main_meter,"mp_check_meter"=>$check_meter, "mp_date_added"=>time(), "mp_added_by"=>user_id(), "mp_region_id"=>$region_id],["mp_id"=>$id2]);
				}

				
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/energy-customer-details/'.$id);
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}

		echo $this->backToCustomerDetails($id);
		?>
		<div class="col-md-12">
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Edit Metering Point Customer for <b><?php echo $this->rgf("customer", $id, "customer_id", "customer_full_name"); ?></b>  Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<br/>
								<div class="col-md-3">
										<div class="form-group">
											
											<label>Region<span class="must">*</span></label>
											<div class="form-line">
												<select name="region_id">
													<option value=""> Select </option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM meter_region");
													foreach($select[0] as $row){
														extract($row);
														if($region_id == $mp_region_id)
															echo '<option selected="selected" value="'.$region_id.'">'.$region_name.'</option>';
														else
															echo '<option value="'.$region_id.'">'.$region_name.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>
								<div class="row">
									<div class="col-md-3">
										<div class="form-group">
											
											<label>Location<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="location" placeholder="" value="<?php echo @$location; ?>">
											</div>
										</div>
									</div>								
									<div class="col-lg-2">
										<div class="form-group">										
											<label>Main Meter<span class="must"></span></label>
											<div class="form-line">
												<input type="text" value="<?php echo $main_meter; ?>" class="form-control" name="main_meter">
											</div>
										</div>
									</div>
									<div class="col-lg-2">
										<div class="form-group">
											<label>Check Meter<span class="must"></span></label>
											<div class="form-line">
												<input type="text" step="" value="<?php echo $check_meter; ?>" class="form-control" name="check_meter">
											</div>
										</div>
									</div>	
									<div class="col-lg-12">
										<input type="hidden"  name="oldname" value="<?php echo $location;?>">
										<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}


	public function editCustomerAction(){
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		echo $this->backToCustomerDetails($id);
		$db = new Db();
		$sql = "SELECT * FROM customer WHERE customer_id = '$id'";
		$select = $db->select($sql);
		extract($select[0][0]);

		$short_name = $customer_short_name;
		$full_name = $customer_full_name;
		$address = $customer_address;
		$default_units = $customer_default_units;
		$vat = $customer_vat;
		$accounts_due = $customer_accounts_due;
		$postal_code = $customer_postal_code;
		$street = $customer_street;
		$email = $customer_email;
		$telephone = $customer_tel;
		$address = $customer_address;

		$image = new Image();
		$image->upload("file");
		$agreement = $image->imageLink();			

		if($image->errorMessage() != "Uploaded" && !empty($image->errorMessage())){
			$errors[] = $image->errorMessage();
		}else{
			if(empty($agreement)){
				$agreement = $customer_agreement;
			}
		}	

		if(isset($_POST['submit'])){
			
			$short_name = $_POST['short_name'];
			$full_name = $_POST['full_name'];
			$address = $_POST['address'];
			$default_units = $_POST['default_units'];
			$vat = $_POST['vat'];
			$currency = $_POST['currency'];
			$accounts_due = $_POST['accounts_due'];
			$postal_code = $_POST['postal_code'];
			$street = $_POST['street'];
			$email = $_POST['email'];
			$telephone = $_POST['telephone'];
			$address = trim($_POST['address']);
			$journal_no = trim($_POST['journal_no']);
			$tin_number = $_POST['tin_number'];
			$cust_type = trim($_POST['cust_type']);

			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();

			if(empty($short_name)){
				$errors[]="Enter Short Name";
			}
			if(empty($full_name)){
				$errors[]="Enter Full Name";
			}
						
			if($this->isThereEdit("customer", ["customer_full_name"=>$full_name, "customer_id"=>$id])){
				$errors[]="Customer Name($full_name) already exists";
			}
			if($this->isThereEdit("customer", ["customer_short_name"=>$short_name, "customer_id"=>$id])){
				$errors[]="Customer Short Name($short_name) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();
				$x = $db->update("customer",
					[
						"customer_full_name"=>$full_name, 
						"customer_short_name"=>$short_name,
						"customer_journal_no"=>$journal_no,
						"customer_address"=>$address, 
						"customer_default_units"=>$default_units, 
						"customer_vat"=>$vat, 
						"customer_currency"=>$currency,
						"customer_accounts_due"=>$accounts_due,
						"customer_street"=>$street,
						"customer_postal_code"=>$postal_code,
						"customer_tel"=>$telephone,
						"customer_email"=>$email,
						"customer_agreement"=>$agreement,
						"customer_tin"=>$tin_number,
						"customer_type"=>$cust_type,
					], ["customer_id"=>$id]);
								
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/energy-customer-details/'.$id);
				}else{
					FeedBack::error($db->error());
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
					Edit Energy Customer Form
				</div>
				<div class="panel-body">
					<div id="must">All field with asterisk(*) are mandatory.</div>

					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" enctype="multipart/form-data" method="post">
								<div class="row">
									<div class="col-md-3">
										<div class="form-group">
											<label>Short Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="short_name" placeholder="" value="<?php echo @$short_name; ?>">
											</div>
										</div>
									</div>
									<div class="col-md-9">
										<div class="form-group">
											<label>Full Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="full_name" placeholder="" value="<?php echo @$full_name; ?>">
											</div>
										</div>
									</div>
									<div class="col-lg-12">
										<div class="row">

											<div class="col-lg-3">
												<div class="form-group">
													
													<label>VAT (%)<span class="must">*</span></label>
													<div class="form-line">
														<input type="number" min="0" step="any" value="<?php echo $vat; ?>" class="form-control" name="vat">
													</div>
												</div>
											</div>
																						
											<div class="col-lg-3">
												<div class="form-group">
													<label>Currency<span class="must">*</span></label>
													<div class="form-line">
														<select name="currency">
															<option value="">--- Select ---</option>
															<?php
															$s = new Db();
															$ss = $s->select("SELECT * FROM currency");
															foreach($ss[0]as $s){
																extract($s);

																if($customer_currency == $currency_id)
																	echo '<option selected="selected" value="'.$currency_id.'">'.$currency_name.' ('.$currency_symbol.')</option>';
																else
																	echo '<option value="'.$currency_id.'">'.$currency_name.' ('.$currency_symbol.')</option>';

															}
															?>
															
														</select>
													</div>
												</div>
											</div>
																						
											<div class="col-md-6">
												<div class="form-group">
													<label>Accounts Due Statement<span class="must">*</span></label>
													<div class="form-line">
														<?php if(empty($accounts_due)){ $accounts_due = "Accounts are due within 45 days of receipt of invoice";}
														?>
														<input class="form-control" type="text" name="accounts_due" placeholder="" value="<?php echo @$accounts_due; ?>">
													</div>
												</div>
											</div>
										</div>
									</div>


									<div class="clearfix"></div>
									<div class="col-lg-12">
										<div class="form-group">
											<label>Address<span class="must">*</span></label>
											<div  class="col-lg-12">
												<div class="form-line">
													<label>Plot No. & Street</label>
													<textarea required style="height:50px;"class="form-control" name="street"><?php echo $street; ?></textarea>
												</div>
												<div class="form-line">
													<label>Postal code & District</label>
													<textarea style="height:50px;"class="form-control" name="postal_code"><?php echo $postal_code; ?></textarea>
												</div>
												<div class="row">
													<div class="col-md-3">
														<div class="form-line">
															<label>Telphone</label>
															<textarea style="height:50px;"class="form-control" name="telephone"><?php echo $telephone; ?></textarea>
														</div>
													</div>
													<div class="col-md-6">
														<div class="form-line">
															<label>Email</label>
															<textarea style="height:50px;"class="form-control" name="email"><?php echo $email; ?></textarea>
														</div>
													</div>
													<div class="col-md-3">
														<div class="form-line">
															<label>Journal No.</label>
															<textarea style="height:50px;"class="form-control" name="journal_no"><?php echo $customer_journal_no; ?></textarea>
														</div>
													</div>
													<div class="col-md-3">
														<label>TIN<span class="must"></span></label>
														<div class="form-line">
														  <input type="text" name="tin_number" value="<?php echo$customer_tin;?>" class="form-control" placeholder="TIN">
													   </div>
													  </div>
													  <div class="col-md-3">
														<label>Customer Type<span class="must"></span></label>
														<div class="form-line">
														  <select class="form-control" name="cust_type">
														  	<option <?php if($customer_type=='B2B') echo "selected";?> value="B2B">B2B</option>
														  	<option <?php if($customer_type=='B2G') echo "selected";?> value="B2G">B2G</option>
														  	<option <?php if($customer_type=='B2C') echo "selected";?> value="B2C">B2C</option>
														  	<option <?php if($customer_type=='B2F') echo "selected";?> value="B2F">B2F</option>
														  </select>
													   </div>
													  </div>

												</div>
											</div>
										</div>
									</div>
									<div class="clearfix"></div>
									<br/>					
									<div class="col-md-9">
										<div class="form-group">
											<label>Attach agreement<span class="must">*</span> <span class="text-muted" style="font-size: 12px;"> Only Pdf documents are recommended</span></label>
											<div class="form-line">
												<input class="form-control" type="file" name="file" placeholder="" value="">
											</div>
										</div>
									</div>
									<br/>

									<div class="col-lg-12">
										<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
									</div>
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}


	public function editUserRoleAction(){
		$id=portion(3);
		$db=new Db();
		$select=$db->select("select * from designation WHERE designation_id='$id'");
		extract($select[0][0]);
		if(isset($_POST['submit'])){
		
			$designation = $_POST['designation'];
			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();
			if(empty($designation)){
				$errors[]="Enter designation";
			}
			if($this->isThereEdit("designation", ["designation_name"=>$designation,"designation_id"=>$id])){
				$errors[]="designation($designation) already exists";
			}
			
			if(empty($errors)){
				$db = new Db();

				$uid = $this->rgf("user_role", $designation_name , "ur_name", "ur_id");
				
				$insert = $db->update("user_role", ["ur_name"=>$designation, "ur_added_by"=>$user_id, "ur_date_added"=>$time], ["ur_id"=>$uid]);

				$x = $db->update("designation",["designation_date_added"=>$time, "designation_name"=>"$designation","designation_added_by"=>$user],["designation_id"=>$id]);
							
				
					
				
				if(!$db->error()){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'designation/all-user-role');
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-4">
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Edit User Role Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
									<label>User Role Name<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" name="designation" placeholder="Enter User Role Name" value="<?php echo @$designation_name; ?>">
									</div>
								</div>
								
								<button onclick = "return confirm('Are you sure you want to save changes?');" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}
	
	public function AllCustomersAction(){
	$access = new AccessRights();
	?>
		<div class="col-md-12">
			<h3>All Energy Customers List</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Short Name</th>';
				echo '<th>Full Name</th>';
				echo '<th>VAT</th>';
				//echo '<th>Default Units</th>';
				echo '<th>Metering Points</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
					
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.ucwords($customer_short_name).'</td>';
					echo '<td>'.ucwords($customer_full_name).'</td>';
					echo '<td>'.number_format($customer_vat).'</td>';
					//echo '<td>'.$this->defaultUnits($customer_default_units).'</td>';
					echo '<td>'.$this->total("metering_point", "mp_customer_id", $customer_id).'</td>';
					
					if($access->sectionAccess(user_id(), $this->page, 'A')){					
						echo '<td>';
						if($access->sectionAccess(user_id(), $this->page, 'A')){
							echo ' &nbsp; '.$this->action('view','customers/energy-customer-details/'.$customer_id, 'View Details');
						}
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

	public function energyCustomerDetailsAction(){
		
		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$id2 =  portion(4);
		if($id2){	
		 $db = new Db();	
		  $ss = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='ENERGY' AND gl_id='$id2'");
		  if($db->num_rows()){
		  	extract($ss[0][0]);
		  }
		}
		//echo "@@@@@@@@@@@@".$gl_account;
			
		$db = new Db();
		$sql = "SELECT * FROM customer WHERE customer_id = '$id'";
		$select = $db->select($sql);
		extract($select[0][0]);

		echo '<div class="row">';
		echo '<div class="col-lg-12">';
		$s = new Services();

		if(empty($year)) $year = date('Y');
		if(empty($month)) $month = date('m');

		//$units = $s->customerAndSchedule($id, $year, $month);
		//$s->energyLinks($id, $year, $month);

		echo '</div>';
		echo '<div class="clearfx"></div>';
		echo '<div class="col-md-12">';
		echo '<ul id="tab_menu" class="" style="margin-top:20px;">';
		echo '<li id="li_1" class="tab active"><a onclick="return false;" onMouseDown="return toggleTabs(1);" href="#">Customer details</a></li>';
		echo '<li id="li_2" class="tab"><a onclick="return false;" onMouseDown="return toggleTabs(2);" href="#">Time band rates</a></li>';
		//echo '<li id="li_3" class="tab"><a onclick="return false;" onMouseDown="return toggleTabs(3);" href="#">Wheeling Charge</a></li> ';
		echo '<li id="li_4" class="tab"><a onclick="return false;" onMouseDown="return toggleTabs(4);" href="#">Metering Points</a></li> '; 
		echo '<li id="li_5" class="tab"><a onclick="return false;" onMouseDown="return toggleTabs(5);" href="#">GL Account</a></li> '; 
		echo '</ul>';

		echo '<div id="tab_section"><!-- content of the tabs is loaded here--></div>';
		echo '</div>';

		
		echo '<div id="1" class="tab_section">';
		echo '<div class="col-md-12">';

		echo '<h4>Customer Details</h4>';
		echo '<table id="table" border="1">';

		echo '<tr>';
		echo '<td width="200px">Short Name:</td>';
		echo '<td>'.$customer_short_name.'</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td>Full Name:</td>';
		echo '<td>'.$customer_full_name.'</td>';
		echo '</tr>';
		
		echo '<tr valign="top">';
		echo '<td>Address:</td>';
		echo '<td>';
		if(empty($customer_street)){
			echo nl2br($customer_address);
		}else{
			echo nl2br($customer_street).'<br/>';
			echo nl2br($customer_postal_code).'<br/>';
			echo nl2br($customer_tel).'<br/>';
			echo nl2br($customer_email).'<br/>';
		}

		echo '</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td>Journal No.:</td>';
		echo '<td>'.$customer_journal_no.'</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td>VAT:</td>';
		echo '<td>'.($customer_vat).'</td>';
		echo '</tr>';
		echo '<tr>';
		echo '<td>Customer Type:</td>';
		echo '<td>'.($customer_type).'</td>';
		echo '</tr>';
		echo '<tr>';
		echo '<td>TIN No.:</td>';
		echo '<td>'.($customer_tin).'</td>';
		echo '</tr>';
		
		
		
		echo '<tr>';
		echo '<td>Agreement:</td>';
		if($customer_agreement){
			echo '<td><a target="_blank" href="'.return_url().$customer_agreement.'">'.$customer_short_name.' Agreement</a></td>';
		}else{
			echo '<td>No Agreement Attached</td>';
		}
		echo '</tr>';

		$s = new Services();
		$tou = $s->tou($customer_id, date(Y), date('m'));
		if(count($tou)){
			echo '<tr>';
			echo '<td>Peak:</td>';
			echo '<td>'.$tou[0].'</td>';
			echo '</tr>';
			
			echo '<tr>';
			echo '<td>Shoulder:</td>';
			echo '<td>'.$tou[1].'</td>';
			echo '</tr>';
			
			echo '<tr>';
			echo '<td>Off Peak:</td>';
			echo '<td>'.$tou[2].'</td>';
			echo '</tr>';
		}
		
		echo '<tr>';
		echo '<td>Currency:</td>';
		if($customer_currency){
			echo '<td>'.$this->rgf("currency",$customer_currency, "currency_id", "currency_name").' ('.$this->rgf("currency",$customer_currency, "currency_id", "currency_symbol").')</td>';
		}else{
			echo '<td>'.$this->rgf("currency",1, "currency_id", "currency_name").' ('.$this->rgf("currency",1, "currency_id", "currency_symbol").')</td>';
		}
		echo '<tr>';
		echo '<td>Accounts Due Statement:</td>';
		echo '<td>'.($customer_accounts_due).'</td>';
		echo '</tr>';
		echo '</tr>';

		echo '</table>';
		echo '<br/>';
		echo '<a class="eagle-load" href="'.return_url().'customers/edit-customer/'.$customer_id.'"><button class="btn btn-success"><i class="fa fa-fx fa-edit"></i> Edit Customer Details</button></a>';
		echo '</div>';
		echo '</div>';
		echo '<script type="text/javascript">_("tab_section").innerHTML = __("tab_section")[0].innerHTML;</script>';
		echo '<div id="2" class="tab_section">';
		echo '<div class="col-lg-12">';

		echo '<h4>Time Band Rates</h4>';
		echo '<table id="table" border="1">';

		echo '<tr valign="bottom">';
		echo '<th rowspan="2">No</th>';
		echo '<th rowspan="2">Peak</th>';
		echo '<th rowspan="2">Shoulder</th>';
		echo '<th rowspan="2">Off Peak</th>';
		echo '<th colspan="2"><center>Effective Date</center></th>';
		echo '</tr>';
		echo '<tr>';
		echo '<th>From </th>';
		echo '<th>To </th>';
		echo '</tr>';

		$select = $db->select("SELECT * FROM time_band WHERE tb_customer_id = '$customer_id' ORDER BY tb_id ASC");
		$no=1;
		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td>'.($no++).'.</td>';
			echo '<td align="right">'.($tb_peak).'</td>';
			echo '<td align="right">'.($tb_shoulder).'</td>';
			echo '<td align="right">'.($tb_off_peak).'</td>';
			echo '<td>'.Feedback::date_s($tb_date_added).'</td>';
			if($tb_effective_date){
				$dd = Feedback::date_s($tb_expiry_date);
			}else{
				$dd = Feedback::date_s($tb_expiry_date);
				//$dd = Feedback::date_s($this->stopDate($tb_customer_id, $tb_id));
			}
			echo '<td>'.$dd.'</td>';
			echo '</tr>';
		}
		extract($select[0][0]);

		echo '</table>';
		echo '<br/>';
		echo '<a class="eagle-load" href="'.return_url().'customers/change-rates/'.$customer_id.'"><button class="btn btn-success">Change Rates</button></a>';
		
		echo '</div>';
		echo '</div>';

		echo '<div id="3" class="tab_section">';
		echo '<div class="col-md-12">';
		echo '<h4>Wheeling Charges</h4>';

		echo '<table id="table" border="1">';
		echo '<tr valign="bottom">';
		echo '<th rowspan="2" width="30px">No.</th>';
		echo '<th rowspan="2">Rate</th>';
		echo '<th colspan="2"><center>Effective Date</center></th>';
		echo '</tr>';

		echo '<tr>';
		echo '<th>From</th>';
		echo '<th>To</th>';
		echo '</tr>';
		$no = 1;
		$db = new Db();
		$select = $db->select("SELECT wc_rate FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");

		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td>'.($no++).'.</td>';
			echo '<td>'.$wc_rate.'</td>';
			echo '<td>'.FeedBack::date_s($wc_date_added).'</td>';
			echo '<td>'.Feedback::date_s($this->stopDate1($wc_customer_id, $wc_id)).'</td>';
			echo '</tr>';
		}

		echo '</table>';
		echo '<br/>';
		echo '<a href="'.return_url().'customers/change-wheeling-charge-rate/'.$customer_id.'"><button class="btn btn-success">Change Wheeling Charge Rate</button></a>';

		echo '</div>';
		echo '</div>';

		echo '<div id="4" class="tab_section">';
		$db = new Db();
		$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '$id'");
		echo '<h4>Metering Points ('.$db->num_rows().') &nbsp;  &nbsp;  &nbsp; '; 

		if($id !=='2036'){
		echo '<a class="eagle-load" href="'.return_url().'customers/add-metering-point/'.$customer_id.'"><button class="btn btn-success btn-xs"><i class="fa fa-fx fa-plus"></i> &nbsp; Add Metering Point </button></a>';
		}
		echo '</h4>';
		echo '<hr/>';
		$no = 1;

		echo '<input type="text" style="padding:0 10px; margin-bottom: 10px;" name="search" autocomplete="off" placeholder="Search Metering Point" id="searchTerm" onkeyup="doSearch2()" onClick="this.select();">';

		echo '<table id="table2" border="1">';
		echo '<thead>';
		echo '<tr>';
		echo '<th width="30px">No.</th>';
		echo '<th>Meter Location</th>';
		echo '<th>Region</th>';
		echo '<th>Main Meter</th>';
		echo '<th>Check Meter</th>';
		//echo '<th>Wheeling Charge(%)</th>';
		//echo '<th>Advance</th>';
		echo '<th style="width:150px;">Action</th>';
		echo '</tr>';
		echo '</thead>';

		foreach($select[0] as $row){
			extract($row);
			echo '<tr>';
			echo '<td>'.($no++).'.</td>';
			echo '<td>'.strtoupper($mp_location).'</td>';
			echo '<td>'.$this->rgf("meter_region", $mp_region_id, "region_id", "region_name").'</td>';
			echo '<td>'.$mp_main_meter.'</td>';
			echo '<td>'.$mp_check_meter.'</td>';
			//echo '<td>'.number_format($mp_wheeling_charge,2).'%</td>';
			//echo '<td>'.$this->advance($mp_advance).'</td>';
			echo '<td style="text-align:right">
			<a href="'.return_url().'customers/delete-metering-point/'.$id.'/'.$mp_id.'" class="btn btn-danger btn-xs"><i class="fa fa-fx fa-times"></i> Delete</a> &nbsp;
			<a href="'.return_url().'customers/edit-metering-point/'.$id.'/'.$mp_id.'" class="eagle-load btn btn-success btn-xs"><i class="fa fa-fx fa-edit"></i> Edit</a></td>';
			echo '</tr>';
		}

		echo '</table>';
		echo '</div>';
		####
		echo '<div id="5" class="tab_section">';
			?>
			<div class="row">
				<div class="col-md-3">
					<div class="form-group">
						<label class="select3 form-label">GL Account</label>
						<select class="form-control" id="gl_accounts">
							<option value="">--select---</option>
							<?php 
							$db = new Db();
		                    $select = $db->select("SELECT * FROM dictionary WHERE d_category IN (9,10, 11, 12, 13, 14, 15) order by d_code ASC, d_id desc");
								foreach ($select[0] as $k) {
									extract($k);
									if($gl_account == $d_id)
									echo'<option selected value="'.$d_id.'">'.$d_code.' - '.$d_description.' ('.$d_code.')</option>';
								else
									echo'<option value="'.$d_id.'">'.$d_code.' - '.$d_description.' ('.$d_code.')</option>';

								}
							?>
						</select>
					</div>
				</div>
				<div class="col-md-2">
					<div class="form-group">
  <!-- Include jQuery -->

						<label class="form-label">GL Label</label>
						<select class="select-gl form-control" id="gl_label">
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
<script>
$(function(){
	$(document).ready(function(){
	    // $('.select-gl').select2();
	});
  });
</script>

  <style>
    .select2-container {
      width: 100% !important;  /* Set width for Select2 */
      display: block;  
      position: relative !important;
  z-index: 1000000 !important;  /* Ensure Select2 dropdown is on top */
    }
  </style>

					</div>
				</div>
				<div class="col-md-2">
					<div class="form-group">
						<label class="form-label">Cr/Dr</label>
						<select class="form-control" id="ct_dt">
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
				            form_data.append('account_entity', 'ENERGY');
				           

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
				                     window.location.replace("<?php echo return_url().'customers/energy-customer-details/'.$id;?>");
				                    $('#save_gl_account_status').html('Successfully Saved');
				                }
				            }
				        
				        }); 
				        }

				    });
				    </script>       
			   
			    <?php
			    echo "<br><br>";
			    $select = $db->select("SELECT * FROM gl_account WHERE gl_cust_id = '$id' AND gl_entity='ENERGY'");
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
						<a href="'.return_url().'customers/delete-gl-account/'.$id.'/'.$gl_id.'" class="btn btn-danger btn-xs"><i class="fa fa-fx fa-times"></i> Delete</a> &nbsp;
						<a href="'.return_url().'customers/energy-customer-details/'.$id.'/'.$gl_id.'" class="eagle-load1 btn btn-success btn-xs"><i class="fa fa-fx fa-edit"></i> Edit</a></td>';
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
	?>

	<?php
	}

	public function deleteMeteringPointAction(){
		$id = portion(3);
		$mp = portion(4);
		if($id == '2019'){
		$db = new Db();
			$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id='2019' AND mp_id = '$mp'");
			if($db->num_rows()){
				extract($select[0][0]);
				$db = new Db();
				$sql = "DELETE FROM metering_point WHERE mp_location = '$mp_location' AND mp_customer_id = '2036'";
				$delete = $db->query($sql);
				$sql = "DELETE FROM metering_point WHERE mp_location = '$mp_location' AND mp_customer_id = '2019'";
				$delete = $db->query($sql);
				FeedBack::redirect(return_url().'customers/energy-customer-details/'.$id);
			}


		}elseif($id == '2036'){
		    $db = new Db();
			$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id='2036' AND mp_id = '$mp'");
			if($db->num_rows()){
				extract($select[0][0]);
				$sql = "DELETE FROM metering_point WHERE mp_location = '$mp_location' AND mp_customer_id = '2036'";
				$delete = $db->query($sql);
				$sql = "DELETE FROM metering_point WHERE mp_location = '$mp_location' AND mp_customer_id = '2019'";
				$delete = $db->query($sql);
				FeedBack::redirect(return_url().'customers/energy-customer-details/'.$id);
			}
		 }else{
		$db = new Db();
		$sql = "DELETE FROM metering_point WHERE mp_id = '$mp' AND mp_customer_id = '$id'";
		$delete = $db->query($sql);

		FeedBack::redirect(return_url().'customers/energy-customer-details/'.$id);
	 }
	}

	public function advance($id){
		if($id == 2) return 'Exports';
		return 'Imports';
	}

	public function stopDate($customer_id, $tb_id){
		$db = new Db();

		$select = $db->select("SELECT TOP 1 * FROM time_band WHERE tb_customer_id = '$customer_id' AND tb_id > $tb_id ORDER BY tb_id ASC");

		if(empty($db->num_rows())){

			return time();
		}

		extract($select[0][0]);
		return $tb_date_added;
	}

	public function stopDate1($customer_id, $wc_id){
		$db = new Db();

		$select = $db->select("SELECT * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' ORDER BY wc_id ASC");

		$select = $db->select("SELECT TOP 1 * FROM wheeling_charge WHERE wc_customer_id = '$customer_id' AND wc_id > $wc_id ORDER BY wc_id ASC");

		if(empty($db->num_rows())){

			return time();
		}

		extract($select[0][0]);
		return $wc_date_added;
	}

	public function changeRatesAction(){
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		echo $this->backToCustomerDetails($id);
		$db = new Db();
		$sql = "SELECT TOP 1 * FROM time_band WHERE tb_customer_id = '$id' ORDER BY tb_id DESC";
		$select = $db->select($sql);
		extract($select[0][0]);

		$peak = $tb_peak;
		$off_peak = $tb_off_peak;
		$shoulder = $tb_shoulder;

		if(isset($_POST['submit'])){
			$peak = $_POST['peak'];
			$off_peak = $_POST['off_peak'];
			$shoulder = $_POST['shoulder'];
			$effective_date = $_POST['effective_date'];
			$expiry_date = $_POST['expiry_date'];

			$errors = array();

			if(empty($peak)){
				$errors[] = "Enter Peak Rate";
			}
			if(empty($off_peak)){
				$errors[] = "Enter Off Peak Rate";
			}
			if(empty($shoulder)){
				$errors[] = "Enter Shoulder Rate";
			}
			if(empty($effective_date)){
				$errors[] = "Select Effective Date";
			}
			if(empty($expiry_date)){
				$errors[] = "Select Expiry Date";
			}

			if(empty($errors)){
				$db = new Db();
				$select = $db->insert("time_band", [
					"tb_customer_id"=>$id,
					"tb_peak"=>$peak,
					"tb_off_peak"=>$off_peak,
					"tb_shoulder"=>$shoulder,
					"tb_shoulder"=>$shoulder,
					"tb_shoulder"=>$shoulder,
					"tb_shoulder"=>$shoulder,
					"tb_added_by"=>user_id(),
					"tb_date_added"=>time(),
					"tb_expiry_date"=>strtotime($expiry_date),
					"tb_effective_date"=>strtotime($effective_date),
				]);

				if(empty($db->error())){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/energy-customer-details/'.$id);
				}else{
					FeedBack::error("Error: ".$db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}

		?>
		<div>
			<h3>Change Rate for <?php echo '<span class="text-primary">'.$this->rgf("customer", $id, "customer_id", "customer_full_name").'</span>'; ?></h3>
		</div>
		<form action="" method="post">
			<div class="col-lg-12">
				<div class="row">
					
					<div class="col-lg-2">
						<div class="form-group">
							<label>Peak<span class="must">*</span></label>
							<div class="form-line">
								<input type="number" min="0" step="any" value="<?php echo $peak; ?>" class="form-control" name="peak">
							</div>
						</div>
					</div>
					<div class="col-lg-2">
						<div class="form-group">
							<label>Shoulder<span class="must">*</span></label>
							<div class="form-line">
								<input type="number" min="0" step="any" value="<?php echo $shoulder; ?>" class="form-control" name="shoulder">
							</div>
						</div>
					</div>
					<div class="col-lg-2">
						<div class="form-group">
							<label>Off Peak<span class="must">*</span></label>
							<div class="form-line">
								<input type="number" min="0" step="any" value="<?php echo $off_peak; ?>" class="form-control" name="off_peak">
							</div>
						</div>
					</div>
					<div class="col-lg-3">
						<div class="form-group">
							<?php if(empty($effective_date)){ $effective_date = date('Y-m-d'); } ?>
							<label>Effective Date<span class="must">*</span></label>
							<div class="form-line">
								<input type="date" min="0" step="any" value="<?php echo $effective_date; ?>" class="form-control" name="effective_date">
							</div>
						</div>
					</div>
					<div class="col-lg-3">
						<div class="form-group">
							<label>Expiry Date<span class="must">*</span></label>
							<div class="form-line">
								<input type="date" min="0" step="any" value="<?php echo $expiry_date; ?>" class="form-control" name="expiry_date">
							</div>
						</div>
					</div>
				</div>
			</div>

			<div class="col-lg-12">
				<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save Changes</button>
			</div>
		</form>

		<?php

	}

	public function changeWheelingChargeRateAction(){
		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT TOP 1 * FROM wheeling_charge WHERE wc_customer_id = '$id' ORDER BY wc_id DESC");
		extract($select[0][0]);

		$rate = $wc_rate;

		if(isset($_POST['submit'])){
			$rate = $_POST['rate'];

			$errors = array();

			if(empty($rate)){
				$errors[] = "Enter Wheeling Charge Rate";
			}

			if(empty($errors)){
				$db = new Db();
				$select = $db->insert("wheeling_charge", [
					"wc_customer_id"=>$id,
					"wc_rate"=>$rate,
					"wc_date_added"=>time(),
					"wc_added_by"=>user_id(),
				]);

				if(empty($db->error())){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'customers/energy-customer-details/'.$id);
				}else{
					FeedBack::error("Error: ".$db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
		}

		?>
		<div>
			<h3>Change Wheeling Charge Rate for <?php echo $this->rgf("customer", $id, "customer_id", "customer_full_name"); ?></h3>
		</div>
		<form action="" method="post">
			<div class="col-lg-12">
				<div class="row">					
					<div class="col-lg-2">
						<div class="form-group">
							<label>Rate<span class="must">*</span></label>
							<div class="form-line">
								<input type="number" min="0" step="any" value="<?php echo $rate; ?>" class="form-control" name="rate">
							</div>
						</div>
					</div>
				</div>
			</div>

			<div class="col-lg-12">
				<button onclick = "" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save Changes</button>
			</div>
		</form>

		<?php

	}

	function lockingAction(){
		echo 'lsdf sdf';
	}

}