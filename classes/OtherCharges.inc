<?php
Class OtherCharges extends BeforeAndAfter{

	public $page = "OTHER CHARGES";
	
	public function __construct(){
		$access = new AccessRights();
		
		if(portion(2) == "all-departments"){
			//if(!$access->sectionAccess(user_id(), $this->page, 'V')){
				//echo '<H1><center style="color:red;">YOU DONT HAVE ACCESS TO THIS PAGE</center></H1>';
				//FeedBack::refresh(1, return_url()."dashboard/index");
			//}
		}else if(portion(2)=="add-department"){
			//if(!$access->sectionAccess(user_id(), $this->page, 'A')){
				//echo '<H1><center style="color:red;">YOU DONT HAVE ACCESS TO THIS PAGE</center></H1>';
				//FeedBack::refresh(1, return_url()."dashboard/index");
			//}
		}else{
			//if(!$access->sectionAccess(user_id(), $this->page2, 'V')){
				//echo '<H1><center style="color:red;">YOU DONT HAVE ACCESS TO THIS PAGE</center></H1>';
			//	FeedBack::refresh(1, return_url()."dashboard/index");
			}
		//}
	}
	
	public function deleteOtherChargeAction(){
		$id = portion(3);
		$this->deletor("other_charges", "oc_id",  $id, 'other-charges/all-other-charges');
	}
	
	public function getLinks(){
		$page = "OTHER CHARGES";
		$links = array(
			// array(
			// 	"link_name"=>"Add Department", 
			// 	"link_address"=>"department/add-department",
			// 	"link_icon"=>"fa-plus",
			// 	"link_page"=>$page,
			// 	"link_right"=>"A",
			// ),
			// array(
			// 	"link_name"=>"View Departments", 
			// 	"link_address"=>"department/all-departments",
			// 	"link_icon"=>"fa-eye",
			// 	"link_page"=>$page,
			// 	"link_right"=>"V",
			// )
		);
		
		return $links;
	}
	
	public function addOtherChargeAction(){
		
		if(isset($_POST['addCharge'])){
			$customer = trim($_POST['customer']);
			$charge_name = trim($_POST['charge_name']);
			$percentage = trim($_POST['percentage']);
			$type = trim($_POST['type']);
			$start_date = trim($_POST['start_date']);
			$end_date = trim($_POST['end_date']);
	

			$errors = array();
			$time = time();
			$user = user_id();
			
			
			$errors = array();

			// if(empty($customer)){
			// 	$errors[] = "Select Customer Name";
			// }
			// if(empty($charge_name)){
			// 	$errors[] = "Enter Charge Name";
			// }
			
				
			if(empty($errors)){
				$db = new Db();
				 $x = $db->insert("other_charges",["oc_date_added"=>$time,"oc_charge_name"=>$charge_name,"oc_added_by"=>$user,"oc_customer_id"=>$customer,"oc_type"=>$type,"oc_start_date"=>strtotime($start_date),"oc_end_date"=>strtotime($end_date),"oc_percentage"=>$percentage]);
				echo $db->error();
				
				if($x){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'other-charges/add-other-charge');
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<?php
			echo"&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'other-charges/all-other-charges'."><i class='fa fa-eye'></i>View Other Charges</a>";?>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Other Charge
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div id="must">All fields with asterisk(*) are mandatory.</div>
								<div class="row">
									
									<div class="col-md-4">
										<div class="form-group">
											<label>Customer<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control" name="customer">
													<option value="">----select----</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM customer ORDER BY customer_full_name ASC");
													foreach ($select[0] as $row) {
														extract($row);
														  if($customer ==$customer_id)
														      echo "<option selected value='".$customer_id."'>".$customer_full_name."(".$customer_short_name.")"."</option>";
														 	else
														 	 echo "<option value='".$customer_id."'>".$customer_full_name."(".$customer_short_name.")"."</option>";	
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="charge_name" placeholder="Enter Charge Name">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Percentage(%)<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="number" step="any" min="0" max="100" name="percentage">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Type<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control" name="type">
													<option value="">---select---</option>
													<option value="Add">Add</option>
													<option value="Substract">Substract</option>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Start Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="start_date" placeholder="">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>End Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" name="end_date" placeholder="Enter Cost Name">
											</div>
										</div>
									</div>
									<div class="col-md-12">
										<div class="form-group">
											<button name="addCharge" type="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
										</div>
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
	
	public function allOtherChargesAction(){
	$access = new AccessRights();
	echo"&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'other-charges/add-other-charge'."><i class='fa fa-plus'></i> Add Other Charges</a>";
	?>
		<div class="col-md-12">
			<h3>Other Charges List</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM other_charges order by oc_date_added desc");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Customer</th>';
				echo '<th>Name</th>';
				echo '<th>Percentage(%)</th>';
				echo '<th>Type</th>';
				echo '<th>Start Date</th>';
				echo '<th>End Date</th>';
				
				

				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'E')){
					echo '<th width="100px">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.$this->rgf("customer",$oc_customer_id,"customer_id","customer_short_name").'</td>';
					echo '<td>'.($oc_charge_name).'</td>';
					echo '<td>'.($oc_percentage).'</td>';
					echo '<td>'.($oc_type).'</td>';	
					echo '<td>'.FeedBack::date_s($oc_start_date).'</td>';
					echo '<td>'.FeedBack::date_s($oc_end_date).'</td>';								
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'E')){
						echo '<td>';
						if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo"<a class='eagle-load1 btn btn-success btn-xs' href=".return_url().'other-charges/edit-other-charge/'.$oc_id.">Edit</a> &nbsp;&nbsp;&nbsp;";
							//echo $this->action('edit','kplc/edit-tariff-schedule/'.$ts_id, 'Edit');
						if($access->sectionAccess(user_id(), $this->page, 'E'))
							//echo $this->action('delete','kplc/delete-tariff-schedule/'.$ts_id, 'Delete');
							echo"<a class='eagle-load1 btn btn-danger btn-xs' href=".return_url().'other-charges/delete-other-charge/'.$oc_id.">Delete</a>";
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
	
	public function editOtherChargeAction(){
		$id = portion(3);
		$db=new Db();
		$select=$db->select("SELECT * from other_charges WHERE oc_id='$id'");
		if($db->num_rows()){
			extract($select[0][0]);
			//print_r($select);
		}
		
      if(isset($_POST['addCharge'])){
			$customer = trim($_POST['customer']);
			$charge_name = trim($_POST['charge_name']);
			$percentage = trim($_POST['percentage']);
			$type = trim($_POST['type']);
			$start_date = trim($_POST['start_date']);
			$end_date = trim($_POST['end_date']);
	

			$errors = array();
			$time = time();
			$user = user_id();
			
			
			$errors = array();

			// if(empty($customer)){
			// 	$errors[] = "Select Customer Name";
			// }
			// if(empty($charge_name)){
			// 	$errors[] = "Enter Charge Name";
			// }
			
				
			if(empty($errors)){
				$db = new Db();
				 $x = $db->update("other_charges",["oc_date_added"=>$time,"oc_charge_name"=>$charge_name,"oc_added_by"=>$user,"oc_customer_id"=>$customer,"oc_type"=>$type,"oc_start_date"=>strtotime($start_date),"oc_end_date"=>strtotime($end_date),"oc_percentage"=>$percentage],["oc_id"=>$id]);
				echo $db->error();
				
				if($x){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'other-charges/edit-other-charge/'.$id);
				}else{
					FeedBack::error();
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="col-md-12">
			<?php
			echo"&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'other-charges/all-other-charges'."><i class='fa fa-eye'></i>View Other Charges</a>";?>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Edit Other Charge
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div id="must">All fields with asterisk(*) are mandatory.</div>
								<div class="row">
									
									<div class="col-md-4">
										<div class="form-group">
											<label>Customer<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control" name="customer">
													<option value="">----select----</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT * FROM customer ORDER BY customer_full_name ASC");
													foreach ($select[0] as $row) {
														extract($row);
														  if($oc_customer_id ==$customer_id)
														     echo "<option selected value='".$customer_id."'>".$customer_full_name."(".$customer_short_name.")"."</option>";
														 	else
														 	  echo "<option value='".$customer_id."'>".$customer_full_name."(".$customer_short_name.")"."</option>";	
													}
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" value="<?php echo$oc_charge_name;?>" name="charge_name" placeholder="Enter Charge Name">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Percentage(%)<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="number" step="any" min="0" value="<?php echo $oc_percentage;?>" max="100" name="percentage">
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Type<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control" name="type">
													<option value="">---select---</option>
													<option <?php if($oc_type =='Add') echo "selected"; ?> value="Add">Add</option>
													<option <?php if($oc_type =='Substract') echo "selected"; ?> value="Substract">Substract</option>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Start Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" value="<?php echo date("Y-m-d",$oc_start_date);?>" name="start_date" placeholder="">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>End Date<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="date" value="<?php echo date("Y-m-d",$oc_end_date);?>" name="end_date" placeholder="Enter Cost Name">
											</div>
										</div>
									</div>
									<div class="col-md-12">
										<div class="form-group">
											<button name="addCharge" type="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
										</div>
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
}