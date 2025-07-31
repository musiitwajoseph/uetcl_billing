<?php
class Users extends BeforeAndAfter{
	public $page = "USERS";
	public $page2 = "HODS";
	
	public $set_class = "";
	public $set_method = "";
	public $set_customer = "";
	public $set_year = "";
	public $set_month = "";

	public function parameters($href){
		$portion = explode('/',str_replace(return_url(), '', $href));

		$this->set_class = $portion[0];
		$this->set_method = $portion[1];
		$this->set_customer = $portion[2];
		$this->set_year = $portion[3];
		$this->set_month = $portion[4];
	}
	public function __construct(){
		$access = new AccessRights();

		$page = "USERS";
		$page2 = "HODS";

		if(portion(2)=="add-user"){
			if(!$access->sectionAccess(user_id(), $page, 'A')){
				echo '<H1><center style="color:red;">YOU DONT HAVE ACCESS TO THIS PAGE</center></H1>';
				FeedBack::refresh(1, return_url());
			}
		}else if(portion(2) == "all-users"){
			if(!$access->sectionAccess(user_id(), $page, 'V')){
				echo '<H1><center style="color:red;">YOU DONT HAVE ACCESS TO THIS PAGE</center></H1>';
				FeedBack::refresh(1, return_url());
			}
		}else if(portion(2) == "all-hods"){
			if(!$access->sectionAccess(user_id(), $page2, 'V')){
				echo '<H1><center style="color:red;">YOU DONT HAVE ACCESS TO THIS PAGE</center></H1>';
				FeedBack::refresh(1, return_url());
			}
		}else if(portion(2) == "changed-password"||portion(2) == "logout"){
			//allowed
		}

		/*else{
			if(!$access->sectionAccess(user_id(), $page2, 'A')){
				echo '<H1><center style="color:red;">YOU DONT HAVE ACCESS TO THIS PAGE</center></H1>';
				//FeedBack::refresh(1, return_url());
			}
		}*/
	}
	
	public function getLinks(){
		$page = "USERS";
		$links = array(
			// array(
			// 	"link_name"=>"Add System User", 
			// 	"link_address"=>"users/add-user",
			// 	"link_icon"=>"fa-user-plus",
			// 	"link_page"=>$page,
			// 	"link_right"=>"A",
			// ),
			array(
				"link_name"=>"View Users", 
				"link_address"=>"users/all-users",
				"link_icon"=>"fa-users",
				"link_page"=>$page,
				"link_right"=>"V",
			),/*
			array(
				"link_name"=>"Add HOD", 
				"link_address"=>"users/add-hod",
				"link_icon"=>"fa-user-plus",
				"link_page"=>$page1,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"View HODs", 
				"link_address"=>"users/all-hods",
				"link_icon"=>"fa-user-secret",
				"link_page"=>$page1,
				"link_right"=>"V",
			)*/
		);
		
		return $links;
	}
	
	public function logout(){
		echo '<h3>Logging Out. Please wait....</h3>';
		session_destroy();
		FeedBack::refresh(1,return_url());
		
		$db = new Db();

		$update = $db->update("sysuser", ["user_last_logged_in"=>time(), "user_online"=>0], [user_id=>user_id()]);
	}
	
	public function deleteHodAction(){
		$id = portion(3);		
		$this->deletor("hod", "hod_id",  $id, 'users/all-hods');
	}

	public function deleteUserAction(){
		$id = portion(3);
		$this->deletor("sysuser", "user_id",  $id, 'users/all-users');
	}
		
	public function AllUsersAction(){
		$access = new AccessRights();
	?>
		<div class="col-md-12">
			<h3>User List</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM sysuser ORDER BY user_surname ASC ");
			
			if(!$select){
				echo $db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr valign="bottom">';
				echo '<th width="30px">No.</th>';
				echo '<th>Name</th>';
				echo '<th>Department</th>';
				//echo '<th>Email</th>';
				echo '<th>User Role</th>';
				echo '<th>Last Logged in</th>';
				echo '<th>Online/<br/>Offline</th>';
				echo '<th>Status</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V')){
					echo '<th width="">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
////////////////////////////////////report step1/////////////////////////////////////
				
				$db_values = array();
				$db_values[] = array(
					
					"No",
					"Name",
					"Department",
					"User Role",
					"Last Logged in",
					"Online/<br/>Offline",
					"Status",
				);
////////////////////////////////////////////////////////////////////////////////				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.($user_surname.' '.$user_othername).'</td>';
					echo '<td>'.$this->rgf("department", $user_department_id, "dept_id", "dept_name").'</td>';
					//echo '<td>'.($user_email).'</td>';
					echo '<td>'.$this->rgf("designation", $user_designation, "designation_id", "designation_name").'</td>';

					echo '<td>'.Feedback::date_fm($user_last_logged_in).'</td>';
					echo '<td>';
					if($user_online){
						$sstt = "Online";
						echo '<span class="text-success">Online</span>';
					}else{	
						$sstt = "Offline";
						echo '<span class="text-danger">Offline</span>';
					}
					
					echo '<td>'.(($user_active)?"Active":"Locked").'</td>';

					echo '</td>';

					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V'))
					{
						echo '<td>';
						
						if($access->sectionAccess(user_id(), $this->page, 'E')){	
							//echo $this->action('edit','users/edit-user/'.$user_id, 'Edit');
							echo '<a class="btn btn-success btn-xs" href="'.return_url().'users/edit-user/'.$user_id.'">Edit</a>';
						}
						echo '&nbsp; &nbsp;';
						if($access->sectionAccess(user_id(), $this->page, 'D')){	
							echo $this->action('view','users/delete-user/'.$user_id, 'Delete Details');
						}
						echo '&nbsp; &nbsp;';
						if($access->sectionAccess(user_id(), $this->page, 'V')){	
							echo $this->action('view','users/view-user-details/'.$user_id, 'View Details');
						}
						echo '</td>';
					}
					echo '</tr>';
					
					$db_values[] = array(
						($i),
						($user_surname.' '.$user_othername),
						$this->rgf("department", $user_department_id, "dept_id", "dept_name"),
						
						$this->rgf("designation", $user_designation, "designation_id", "designation_name"),

						Feedback::date_fm($user_last_logged_in),
						$sstt,
						
						(($user_active)?"Active":"Locked"),

						
						); 
				}
				echo '</tbody>';
				
				echo '</table>';

//////////////////////////////report step3/////////////////////////////////////
				
				$t = new TableCreator();
				$heading = "SYSTEM USER LIST"; //CHANGE
				$t->open($this->full_name(user_id()), $heading);
				$t->thd($db_values);
				$t->close();
				$t->results();

				$e = new Exporter();
				echo $e->getDisplay($heading, $t->results());
////////////////////////////////////////////////////////////////////////////////////

			}
			
			?>
		</div>
		<?php
	}
	
	public function ViewUserDetailsAction(){
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		//$id = ($this->set_method)?$this->set_method:portion(3);
		//$id = portion(3);
		echo '<div class="col-lg-12"><a href="'.return_url().'users/edit-user/'.$id.'" class="btn btn-success btn-md pull-right"><i class="fa fa-fx fa-edit"></i> Edt User Details</a></div>';
		echo '<div class="clearfix"></div>';
		$this->detailsOfUsers($id);
		echo '<br/></br/>';
	}

	public function detailsOfUsers($id){
	$n = new Db();
		$nn = $n->select("SELECT * FROM sysuser WHERE user_id = '$id'");
		extract($nn[0][0]);

		$access = new AccessRights();
	?>
		<div class="col-md-12">
			<h3>User Account Details</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM sysuser WHERE user_id = '$id' ORDER BY user_surname ASC ");
			
			if(!$select){
				echo $db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr valign="bottom">';
				echo '<th width="30px">No.</th>';
				echo '<th width="150px">Name</th>';
				echo '<th>Value</th>';
				
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Account Status</td>';	
					echo '<td>'.(($user_active)?"Active":"Locked").'</td>';				
					echo '</tr>';

					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Account Created On</td>';	
					echo '<td>'.Feedback::date_fm($user_date_added).'</td>';				
					echo '</tr>';

					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Last Logged in</td>';	
					echo '<td>'.Feedback::date_fm($user_last_logged_in).'</td>';				
					echo '</tr>';

					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Password Last changed</td>';	
					echo '<td>'.Feedback::date_fm($user_last_changed).'</td>';				
					echo '</tr>';


					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Name</td>';	
					echo '<td>'.($user_surname.' '.$user_othername).'</td>';				
					echo '</tr>';
					
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Username</td>';	
					echo '<td>'.($user_name).'</td>';				
					echo '</tr>';
					
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Email</td>';	
					echo '<td>'.($user_email).'</td>';				
					echo '</tr>';
					
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Phone</td>';	
					echo '<td>'.($user_telephone).'</td>';				
					echo '</tr>';
					
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>User Role:</td>';	
					echo '<td>'.$this->rgf("designation", $user_designation, "designation_id", "designation_name").'</td>';				
					echo '</tr>';
					
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Gender:</td>';	
					echo '<td>';
					echo	($user_gender)?"Male":"Female";
					echo '</td>';				
					echo '</tr>';
					
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Online/Offline</td>';	
					echo '<td>'.(($user_online)?"Online":"Offline").'</td>';				
					echo '</tr>';
					
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>Department</td>';	
					echo '<td>'.$this->rgf("department", $user_department_id, "dept_id", "dept_name").'</td>';				
					echo '</tr>';
					
					echo '<tr><td colspan="3"></td></tr>';				
				}
				echo '</tbody>';
				
				echo '</table>';
			}
			
			?>
		</div>
		<?php
	}
	public function addUserAction(){
		$msg_pass = "Password should be at least 7 characters in length and should include at least an upper case letter(A-Z), lower case letter(a-z), number(0-9), and special character.(@ ? / # $ %) ";
		$db = new Db();
		if(isset($_POST['submit'])){
			
			$check_number = addslashes(ucwords(strtolower($_POST['check_number'])));	
			$surname = addslashes(ucwords(strtolower($_POST['surname'])));			
			$othername = addslashes(ucwords(strtolower($_POST['othername'])));			
			$username = addslashes($_POST['username']);			
			$email = addslashes($_POST['email']);				
			$telephone = addslashes($_POST['telephone']);
			//$password = addslashes($_POST['password']);	
			//$cpassword = addslashes($_POST['cpassword']);	
			
			$user_section_id = addslashes($_POST['user_section_id']);	
			$user_department_id = addslashes($_POST['user_department_id']);	
			$user_branch_id = addslashes($_POST['user_branch_id']);

			
			$user_gender = addslashes($_POST['user_gender']);
			$user_designation = addslashes($_POST['designation']);
			
			$user_role = addslashes($_POST['user_role']);
			
			$isdriver = addslashes($_POST['isdriver']);

			$user_id = user_id();
			$time = time();
			
			$errors = array();
			
			if(empty($username)){
				$errors[] = "Please Enter username";
			}
			if(empty($othername)){
				$errors[] = "Please fill your othername";
			}
			if(empty($user_designation)){
				$errors[] = "Select User Role";
			}
			if(empty($email)){
				$errors[] = "Please Enter your Email Address";
			}
			if(empty($telephone)){
				$errors[] = "Enter telephone number";
			}

			if(!empty($user_branch_id)&&$user_branch_id == static_office()){
				if(empty($territory)){
					$errors[] = "Select Territory";
				}
				if(empty($area_office)){
					$errors[] = "Select Area Office";
				}
			}
			
			/*
			if(empty($user_gender)){
				$errors[] = "Select your gender";
			}
			if(empty($password)){
				$errors[] = "Enter the password";
			}
			if($this->passwordStrength($password)){
				$errors[] = $msg_pass;
			}
			if($password!=$cpassword){
				$errors[]="Password should match with Confirm Password";
			}
			*/

			if(empty($username)){
				$errors[] = "Enter Username";
			}

			
			if($this->isThere("sysuser", ["user_name"=>$username])){
				$errors[] = "username $username already exists";
			}
			
			if($this->isThere("sysuser", ["check_number"=>$check_number])){
				//$errors[] = "Check Number $check_number already exists";
			}

			
			$designation_name = $this->rgf("designation", $user_designation, "designation_id", "designation_name");
				
			$user_role = $this->rgf("user_role", $designation_name , "ur_name", "ur_id");
			$user_designation1212 = $user_designation;

			if(empty($errors)){
				$password = Feedback::password_generator();

				$insert = $db->insert("sysuser", ["user_name"=>$username, "user_surname"=>$surname,
				"user_othername"=>$othername,
				"user_status"=>1,
				"user_designation"=>$user_designation,
				"user_branch_id"=>$user_branch_id,
				"user_email"=>$email,
				"user_telephone"=>$telephone,
				"user_gender"=>$user_gender,
				"user_section_id"=>$user_section_id,
				"user_department_id"=>$user_department_id,
				"user_password"=>$password,
				"user_date_added"=>$time,
				"user_added_by"=>$user_id,
				"user_role"=>$user_role,
				"user_territory_id"=>$territory,
				"user_area_office_id"=>$area_office,
				"user_forgot_password"=>1,
				"user_active"=>1,
				"check_number"=>$check_number,
				]);
				
				echo $db->error();
				if($insert){

					$msg = array();
	                $msg[] = "Hello $user_surname $othername, ";
	                $msg[] = "";
	                $msg[] = "Your account has been successfully created by : ".$this->full_name(user_id());
	                $msg[] = "";
	                $msg[] = "Username: $username";
	                
	                if(!empty($password)){
	                	 $msg[] = "Password: $password";
	            	}
	               
	                $mssg[] = "";
	                $message[] = "Thank You.";

	                $to = $email;
	                $subject = "UETCL ACCOUNT CHANGES";
	                $message = implode("\r \n <br/>", $msg);
	                Feedback::sendmail($to,$subject,$message,$name);

					FeedBack::success();
					FeedBack::refresh();
				}else{
					FeedBack::error('Not Saved, '.$db->error());
				}
			}else{
				FeedBack::errors($errors);
			}
					
		}	
		
		//echo $this->branch_name(3);
			
		?>
		<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add User Form
				</div>
				<div class="panel-body">
					<div class="row">

						<form role="form" action="" method="post">
							<div class="col-md-12">
								<div id="must">All field with asterisk(*) are mandatory.</div>
							</div>
							<div class="clearfix"></div>
							<br/>
							<div class="col-lg-3">
								<div class="form-group">
									<label>Surname<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" name="surname" value="<?php echo @$surname; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Othername(s)<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" name="othername" value="<?php echo @$othername; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Telephone Phone<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" name="telephone" value="<?php echo @$telephone; ?>">
									</div>
								</div>
							</div>							
							<div class="col-lg-3">
								<div class="form-group">
									<label>Email Address<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="email" autocomplete="off" name="email" value="<?php echo @$email; ?>">
									</div>
								</div>
							</div>							
							<div class="col-lg-2">
								<div class="form-group">
									<label>Gender<span class="must"></span></label>
									<div class="form-line">
										<select name="user_gender">
											<?php 
											if($user_gender ==1)
												echo '<option selected="selected" value="1">Male</option>';
											else
												echo '<option value="1">Male</option>';

											if($user_gender ==0)
												echo '<option selected="selected" value="0">Female</option>';
											else
												echo '<option value="0">Female</option>';
											?>
										</select>
									</div>
								</div>
							</div>
							<div class="clearfix"></div>
													
							<div class="col-lg-4" style="display:none;" title="<?php echo $msg_pass; ?>" class="form-control" type="password">
								<div class="form-group">
									<label>Password<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="password" name="password" value="<?php echo @$password; ?>">
									</div>
								</div>
							</div>							
							<div class="col-lg-4" style="display:none;">
								<div class="form-group">
									<label>Confirm Password<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="password" name="cpassword" value="<?php echo @$cpassword; ?>">
									</div>
								</div>
							</div>	
							<div class="clearfix"></div>							
									<div class="col-lg-3">
										<div class="form-group">
											<label>Department<span class="must">*</span></label>
											<div class="form-line">
												<select name="user_department_id" id="department" onchange=" officeCode('department'); officeCodeRole('departmentRoles'); ">
													<option value="">--</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT dept_id, dept_name FROM department WHERE dept_name IS NOT NULL AND dept_status = 1 ORDER BY dept_name ASC");
													foreach($select[0] as $row){
														extract($row);
														if($dept_id == $user_department_id)
															echo '<option value="'.$dept_id.'" selected="selected">'.$dept_name.'</option>';
														else
															echo '<option value="'.$dept_id.'">'.$dept_name.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>							
									
							<div class="col-lg-2">
								<div class="form-group">
									<label>Username<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" name="username" value="<?php echo @$username; ?>">
									</div>
								</div>
							</div>	
							<div class="col-lg-4">
								<div class="form-group">
									<label>User Role<span class="must">*</span></label>
									<div class="form-line">
										<select name="designation" id="" onclick="return designation();">
											<?php
											$hod = static_hod_id();
											$db = new Db();

											$select = $db->select("SELECT user_designation FROM sysuser WHERE user_designation = '$hod' AND user_department_id = '$user_department_id' AND user_active = 1;");
											$all_hods = array();
											if($db->num_rows()){
												foreach($select[0] as $row1){
													extract($row1);
													$all_hods[] = $user_designation;
												}
											}

											$sql = "SELECT user_designation FROM sysuser WHERE user_active = 1 AND ( user_designation = ".static_transport_officer()." OR user_designation = ".static_accounts_officer()."  OR user_designation = ".static_md()." OR user_designation = ".static_hr_and_admin()." OR user_designation = ".static_cfo()." OR user_designation = ".static_super()." OR user_designation = ".static_hos().")"; 

											$select = $db->select($sql);

											if($db->num_rows()){
												foreach($select[0] as $row1){
													extract($row1);
													$all_hods[] = $user_designation;
												}
											}

											$db = new Db();

											$select = $db->select("SELECT designation_id, designation_name FROM designation ORDER BY designation_name ASC");
											
											if($db->num_rows()){
												echo '<option value=""> -- </option>';
												foreach($select[0] as $row){
												extract($row);
													//if(in_array($designation_id, $all_hods) )
													{

													//}else{
														//if(!empty($user_department_id))
														{

															if($user_designation1212 == $designation_id){
															 	echo '<option selected="selected" value="'.$designation_id.'">'.$designation_name.'</option>';
															}else{
																echo '<option value="'.$designation_id.'">'.$designation_name.'</option>';
															}
														}
													}
												}
											}
											?>
										</select>
									</div>
								</div>
							</div>
							

							<script>
								function officeCode(atat){
									
									var atatValue = document.getElementById(atat).value;
									
									if(atat == "office"){
										var target = "department";
									}else if(atat == "department"){
										var target = "section";
									}else if(atat == "territory"){
										var target = "area_office";
									}		

									var request = new XMLHttpRequest();
									request.open("POST", "/UETCL/classes/office_details.php", true);
									request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
									request.onreadystatechange = function() {
										if(request.readyState == 4 && request.status == 200) {
											//4    = The connection is complete, the data was sent or retrieved.
											document.getElementById(target).innerHTML = request.responseText;

										}
									}
									request.send("type="+atat+"&value="+atatValue+"&target="+target);
									
								}
								function officeCode12(atat){

									var atatValue = document.getElementById("office").value;
									
									var target = "territory";

									var request = new XMLHttpRequest();
									request.open("POST", "/UETCL/classes/office_details.php", true);
									request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
									request.onreadystatechange = function() {
										if(request.readyState == 4 && request.status == 200) {
											//4    = The connection is complete, the data was sent or retrieved.
											document.getElementById(target).innerHTML = request.responseText;

										}
									}
									request.send("type="+atat+"&value="+atatValue+"&target="+target);
									
								}
								function officeCodeRole(atat){

									var atatValue = document.getElementById("department").value;
									var target = "designation";

									var request = new XMLHttpRequest();
									request.open("POST", "/UETCL/classes/office_details.php", true);
									request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
									request.onreadystatechange = function() {
										if(request.readyState == 4 && request.status == 200) {
											//4    = The connection is complete, the data was sent or retrieved.
											document.getElementById(target).innerHTML = request.responseText;

										}
									}
									request.send("type="+atat+"&value="+atatValue+"&target="+target);
									
								}

							</script>								
							
							<div class="clearfix"></div>
							<div class="col-lg-12">
								<button type="submit" onclick="return confirm('Are you sure you want to save.');"name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</div>
						</form>
					</div>
				</div>
			</div>
		</div>
		<?php
		

	}
	
	public function editUserAction(){

		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT * FROM sysuser WHERE user_id = '$id'");
		foreach($select[0] as $row){
			extract($row);
			$ud = $user_designation;
		}


		$msg_pass = "Password should be at least 7 characters in length and should include at least an upper case letter(A-Z), lower case letter(a-z), number(0-9), and special character.(@ ? / # $ %) ";
		$db = new Db();
		if(isset($_POST['submit'])){
			
			$check_number = addslashes(ucwords(strtolower($_POST['check_number'])));
			$surname = addslashes(ucwords(strtolower($_POST['surname'])));			
			$othername = addslashes(ucwords(strtolower($_POST['othername'])));			
			$username = addslashes($_POST['username']);			
			$email = addslashes($_POST['email']);				
			$telephone = addslashes($_POST['telephone']);

			//$password = addslashes($_POST['password']);				
			//$cpassword = addslashes($_POST['cpassword']);
			/*
			if(!empty($password) || !empty($cpassword)){ 
				if(empty($password)){
					$errors[] = "Enter the password";
				}
				if($password!=$cpassword){
					$errors[]="Password should match with Confirm Password";
				}
				if($this->passwordStrength($password)){
					$errors[] = "Password should be at least 7 characters in length and should include at least one upper case letter, one number, and one special character.";
				}
			}*/	
			
			$user_section_id = addslashes($_POST['user_section_id']);	
			$user_department_id = addslashes($_POST['user_department_id']);	
			$user_branch_id = addslashes($_POST['user_branch_id']);

			
			$user_gender = addslashes($_POST['user_gender']);
			$user_designation = addslashes($_POST['designation']);
			$userdesign = $user_designation;
			
			$user_role = addslashes($_POST['user_role']);
			
			$user_active = addslashes($_POST['status']);

			$user_id = user_id();
			$time = time();
			
			$errors = array();

			if(empty($username)){
				$errors[] = "Please Enter username";
			}
			if(empty($othername)){
				$errors[] = "Please fill your othername";
			}
			if(empty($user_designation)){
				$errors[] = "Select User Role";
			}
			if(empty($email)){
				$errors[] = "Please Enter your Email Address";
			}
			if(empty($telephone)){
				$errors[] = "Enter telephone number";
			}
			/*
			if(empty($user_gender)){
				$errors[] = "Select your gender";
			}*/
			
			if($this->isThereEdit("sysuser", ["user_name"=>$username, "user_id"=>$id])){
				$errors[] = "username $username already exists";
			}
						
			$ud = $user_designation;
			$userdesign = $user_designation;

			$designation_name = $this->rgf("designation", $user_designation, "designation_id", "designation_name");
				
			$user_role = $this->rgf("user_role", $designation_name , "ur_name", "ur_id");

			




			if(empty($errors)){
			

			//if($password==$cpassword && (!empty($password) || !empty($cpassword)))
			if(1)
			{
				$insert = $db->update("sysuser", [
				"user_name"=>$username, 
				"user_surname"=>$surname,
				"user_othername"=>$othername,
				"user_status"=>1,
				"user_designation"=>$user_designation,
				"user_branch_id"=>$user_branch_id,
				"user_email"=>$email,
				"user_telephone"=>$telephone,
				"user_gender"=>$user_gender,
				"user_section_id"=>$user_section_id,
				"user_department_id"=>$user_department_id,
				//"user_password"=>$password,
				"user_date_added"=>$time,
				"user_added_by"=>$user_id,
				"user_role"=>$user_role,
				"user_territory_id"=>$territory,
				"user_area_office_id"=>$area_office,
				"user_active"=>$user_active,
				"check_number"=>$check_number,
				], ["user_id"=>$id]);



			}

			$msg = array();
            $msg[] = "Hello $user_surname $user_othername, ";
            $msg[] = "";
            $msg[] = "Your Login credentials have been successfully changed by :".$this->full_name(user_id());
            $msg[] = "";
            $msg[] = "Username: $username";
            $msg[] = "Password: $password";
            $message[] = "Thank You.";

            $to = $user_email;
            $subject = "UETCL ACCOUNT CHANGES";
            $message = implode("\r \n <br/>", $msg);
            Feedback::sendmail($to,$subject,$message,$name);

			echo $db->error();
			if($insert){
				FeedBack::success();
				FeedBack::refresh(3, return_url()."users/all-users");
			}else{
				FeedBack::error('Not Saved, '.$db->error());
			}
			}else{
				FeedBack::errors($errors);
			}
					
		}	
		
		//echo $this->branch_name(3);
			
		?>
		<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Edit User Form
				</div>
				<div class="panel-body">
					<div class="row">

						<form role="form" action="" method="post">
							<div class="col-md-12">
								<div id="must">All field with asterisk(*) are mandatory.</div>
							</div>
							<div class="clearfix"></div>
							<br/>
							<div class="clearfix"></div>
							<div class="col-lg-3">
								<div class="form-group">
									<label>Surname<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" name="surname" value="<?php echo @$user_surname; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Othername(s)<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" name="othername" value="<?php echo @$user_othername; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Telephone Phone<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" name="telephone" value="<?php echo @$user_telephone; ?>">
									</div>
								</div>
							</div>							
							<div class="col-lg-3">
								<div class="form-group">
									<label>Email Address</label>
									<div class="form-line">
										<input class="form-control" type="email" autocomplete="off" name="email" value="<?php echo @$user_email; ?>">
									</div>
								</div>
							</div>							
							<div class="col-lg-2">
								<div class="form-group">
									<label>Gender</label>
									<div class="form-line">
										<select name="user_gender">
										<?php 
											if($user_gender ==1)
												echo '<option selected="selected" value="1">Male</option>';
											else
												echo '<option value="1">Male</option>';

											if($user_gender ==0)
												echo '<option selected="selected" value="0">Female</option>';
											else
												echo '<option value="0">Female</option>';
										?>
										</select>
									</div>
								</div>
							</div>
							<div class="clearfix"></div>
							
									<div class="col-lg-3">
										<div class="form-group">
											<label>Department<span class="must">*</span></label>
											<div class="form-line">
												<select name="user_department_id" id="department" onchange=" officeCode('department'); officeCodeRole('departmentRoles'); ">
													<option value="">--</option>
													<?php
													$db = new Db();
													$select = $db->select("SELECT dept_id, dept_name FROM department WHERE dept_name IS NOT NULL AND dept_status = 1 ORDER BY dept_name ASC");
													foreach($select[0] as $row){
														extract($row);
														if($dept_id == $user_department_id)
															echo '<option value="'.$dept_id.'" selected="selected">'.$dept_name.'</option>';
														else
															echo '<option value="'.$dept_id.'">'.$dept_name.'</option>';
													}
													?>
												</select>
											</div>
										</div>
									</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Username<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" name="username" value="<?php echo @$user_name; ?>">
									</div>
								</div>
							</div>	
							<div class="col-lg-4">
								<div class="form-group">
									<label>User Role<span class="must">*</span></label>
									<div class="form-line">
										<select name="designation" id="" onclick="return designation();">
											<?php
											$hod = static_hod_id();
											$db = new Db();

											$select = $db->select("SELECT user_designation FROM sysuser WHERE user_designation = '$hod' AND user_department_id = '$user_department_id' AND user_active = 1;");
											$all_hods = array();
											if($db->num_rows()){
												foreach($select[0] as $row1){
													extract($row1);
													$all_hods[] = $user_designation;
												}
											}

											$sql = "SELECT user_designation FROM sysuser WHERE user_active = 1 AND ( user_designation = ".static_transport_officer()." OR user_designation = ".static_accounts_officer()."  OR user_designation = ".static_md()." OR user_designation = ".static_hr_and_admin()." OR user_designation = ".static_cfo()." OR user_designation = ".static_super()." OR user_designation = ".static_hos().")"; 

											$select = $db->select($sql);

											if($db->num_rows()){
												foreach($select[0] as $row1){
													extract($row1);
													$all_hods[] = $user_designation;
												}
											}

											$db = new Db();

											$select = $db->select("SELECT designation_id, designation_name FROM designation ORDER BY designation_name ASC");
											
											if($db->num_rows()){
												echo '<option value=""> -- </option>';
												foreach($select[0] as $row){
												extract($row);
													//if(in_array($designation_id, $all_hods) )
													{

													//}else{
														//if(!empty($user_department_id))
														{

															if($user_designation == $designation_id){
															 	echo '<option selected="selected" value="'.$designation_id.'">'.$designation_name.'</option>';
															}else{
																echo '<option value="'.$designation_id.'">'.$designation_name.'</option>';
															}
														}
													}
												}
											}
											?>
										</select>
									</div>
								</div>
							</div>						
								
							<div class="clearfix"></div>
							<div class="col-lg-2">
								<div class="form-group">
							
									<label>Status:<span class="must"></span></label>
									<div class="form-line">
										<?php
										echo '<select name="status">';

										if($user_active == 0)
											echo '<option value="0" selected="selected">'.$this->show2.'</option>';
										else
											echo '<option value="0">'.$this->show2.'</option>';

										if($user_active == 1)
											echo '<option value="1" selected="selected">'.$this->show1.'</option>';
										else
											echo '<option value="1">'.$this->show1.'</option>';
										echo '</select>';
										?>
									</div>
								</div>
							</div>
								
							<div class="clearfix"></div>
							<div class="col-lg-12">
								<button type="submit" onclick="return confirm('Are you sure you want to save.');" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</div>
						</form>
					</div>
				</div>
			</div>
		</div>
		<?php
		

	}
	public function passwordStrength($password){
		
		// Validate password strength
		$uppercase = preg_match('@[A-Z]@', $password);
		$lowercase = preg_match('@[a-z]@', $password);
		$number    = preg_match('@[0-9]@', $password);
		$specialChars = preg_match('@[^\w]@', $password);
		
		if(!$uppercase || !$lowercase || !$number || !$specialChars || strlen($password) < 7) {
		    return true;
		}else{
		    return false;
		}

	}

	public function changedPassword(){
		$msg_pass = "Password should be at least 7 characters in length and should include at least an upper case letter(A-Z), lower case letter(a-z), number(0-9), and special character.(@ ? / # $ %) ";
		
		$user_id = user_id();

		if($this->rgf("sysuser", $user_id, "user_id", "user_forgot_password")){
			Feedback::error("Please change your password.");
		}


		if(isset($_POST['submit'])){
		
			$email_password = $_POST['email_password'];
			$password = $_POST['password'];
			$cpassword = $_POST['cpassword'];
			$username = $_POST['username'];
			$user_id = user_id();
			$db = new Db();
			
			$errors = array();

			if(empty($username)){
				$errors[] = "Enter Username";
			}

			if(empty($email_password)){
				$errors[] = "Enter Old password";
			}

			if((!empty($password) || !empty($cpassword)) && $cpassword != $password){
				if(empty($password)){
					$errors[] = "Enter password";
				}

				if(empty($cpassword)){
					$errors[] = "Confirm password";
				}

				if($cpassword != $password){
					$errors[] = "New password does not Match with Confirm password";
				}
			}elseif($this->passwordStrength($password) && !empty($password)){
				$msg_pass = "Password should be at least 7 characters in length and should include at least an upper case letter(A-Z), lower case letter(a-z), number(0-9), and special character.(@ ? / # $ %) ";
				$errors[] = "$msg_pass";
			}
			
			if($this->isThereEdit("sysuser", ["user_name"=>$username, "user_id"=>$user_id])){
				$errors[] = "username $username already exists";
			}
			
			if(empty($errors)){
				$select = $db->select("SELECT * FROM sysuser WHERE user_id = $user_id AND user_password = '$email_password';");

				if($db->num_rows()){
					$db = new Db();

					if(empty($password)){
						$x = $db->update("sysuser", ["user_forgot_password"=>NULL, "user_name"=>$username, "user_last_changed"=>time()],["user_id"=>$user_id]);
					}else{
						$x = $db->update("sysuser", ["user_forgot_password"=>NULL, "user_password"=>$password, "user_name"=>$username, "user_last_changed"=>time()],["user_id"=>$user_id]);
					}

					$msg = array();
                    $msg[] = "Hello $user_surname $user_othername, ";
                    $msg[] = "";
                    $msg[] = "Your Login credentials have been successfully changed by :".$this->full_name(user_id());
                    $msg[] = "";
                    $msg[] = "Username: $username";

                    if(empty($password)){
                    	 $msg[] = "Password: $password";
                	}
                   
                    $mssg[] = "";
                    $message[] = "Thank You.";

                    $to = $user_email;
                    $subject = "UETCL ACCOUNT CHANGES";
                    $message = implode("\r \n <br/>", $msg);
                    Feedback::sendmail($to,$subject,$message,$name);

                    


				}else{
					$errors[] = "Wrong Old password that was stored in the system";
				}
			}
			
						
			

			
				
			if(empty($errors)){
			//$x = $db->insert("department",["dept_date_added"=>$time, "dept_name"=>$department,"dept_added_by"=>$user]);
			
			if($x){
				$this->logout();
				FeedBack::success();
				FeedBack::refresh(3);
			}else{
				FeedBack::error();
			}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>

		<div class="col-md-12">
			<div class="col-lg-12" style="display:none;">
			<div class="panel panel-default">
				<div class="panel-heading">
					Change Username & Password:
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="row">
									<div class="col-lg-12" id="must">All field with asterisk(*) are mandatory.</div>	
									<div class="col-lg-3">
										<div class="form-group">
											<label>User name<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="username" placeholder="Enter username" value="<?php echo $this->rgf("sysuser", user_id(), "user_id", "user_name"); ?>">
											</div>
										</div>
									</div>									
									<div class="col-lg-3">
										<div id="must">&nbsp;</div>
										<div class="form-group">
											<label>Email Old password</label>
											<div class="form-line">
												<input class="form-control" type="password" name="email_password" placeholder="Password from Your email" value="">
											</div>
										</div>
									</div>
									
									<div class="col-lg-3" title="<?php echo $msg_pass; ?>">	
										<div id="must">&nbsp;</div>
										<div class="form-group">
											<label>Enter new password</label>
											<div class="form-line">
												<input class="form-control password" type="password" name="password" placeholder="password" value="" title="<?php echo $msg_pass; ?>">
											</div>
											<div class="password-hint" style="color:red;font-size:12px;"><br/><b>Hint:</b><br/><?php echo $msg_pass; ?></div>
											<script type="text/javascript">
												$(document).ready(function(){
													$('.password-hint').hide();
													$('.password').focusin(function(){
														$('.password-hint').show();
													});
													$('.password').focusout(function(){
														$('.password-hint').hide();
													});
												});
											</script>
										</div>
									</div>									
									<div class="col-lg-3">
										<div class="form-group">
											<div id="must">&nbsp;</div>
											<label>Confirm password</label>
											<div class="form-line">
												<input class="form-control" type="password" name="cpassword" placeholder="Confirm Password" value="" title="<?php echo $msg_pass; ?>">
											</div>
										</div>
									</div>
									<div class="col-md-12">
										<button type="submit" onclick="return confirm('Are you sure you want to save.');" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
									</div>
							
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>

		<div class="col-md-12">
			<?php
			echo '<br/><br/>';
			$this->detailsOfUsers(user_id());
			?>
		</div>

	<?php
	}

	public function gender($sex){
		if($sex)
			return "Male";
		else
			return "Female";
	}

	public function help(){
		?>

		<a target="_blank" href="<?php echo return_url().'users/help-energy'; ?>">Energy Tutorials</a>
		<?php
	}

	public function helpEnergy(){
		?>
			
		<div class="row">			
				<h3 class="text-primary">Energy Tutorials</h3><br/>
					<?php 
					$videos = array(
						"01. ADD CUSTOMER",
						"02. ADD METERING POINT",
						"03. UPLOAD READINGS",
						"04. ADD READINGS",
						"05. DEBIT AND CREDIT NOTE",
						"06. DIRECTING WHEELING CHARGE",
						"07. LOCKING",
						"08. READINGS STATUS or SUMMARY",
						"09. How to extract readings",
						"10. FORMULAE BUILBER",
					);
					?>


					<div class="col-md-9">
						<?php 
							$video_portion = str_replace('_', ' ', str_replace('-', '.', portion(3)));

							$video_portion = ($video_portion)?$video_portion:$videos[0];
						?>
						<div>
							<p class="" style="padding:0; margin-bottom: 10px;"><b><?php echo strtoupper($video_portion); ?></b></p>
							<video controls autoplay style="width:100%">
								<source type="video/mp4" src="<?php echo return_url().'videos/training/tutorials/'.$video_portion.'.mp4'; ?>"></source>
							</video>
						</div>
					</div>

					<?php
					echo '<div class="col-md-3">';
					echo '<p class="" style="padding:0; margin-bottom: 10px;"><b>PLAYLIST</b></p>';
					foreach($videos as $video){ 
						if($video == $video_portion){
						?>
						
						<div class="panel panel-primary" style="padding:5px; margin:5px; background-color: blue; color:#FFF;">
							<div>
								<p style="font-size: 12px; padding:0; margin:0;"><b><?php echo strtoupper($video); ?></b></p>
								<!-- <video controls style="width:100%">
									<source type="video/mp4" src="<?php echo return_url().'videos/training/tutorials/'.$video.'.mp4'; ?>"></source>
								</video> -->
							</div>
						</div>
					<?php }else{ ?><a href="<?php echo return_url().'users/help-energy/'.str_replace(' ', '_', str_replace('.', '-', $video)); ?>">
						<div class="panel panel-default" style="padding:5px; margin:5px; background-color: gray; color:#FFF;">
							<div>
								<p class="" style="font-size: 12px; padding:0; margin:0;"><b><?php echo strtoupper($video); ?></b></p>
								<!-- <video controls style="width:100%">
									<source type="video/mp4" src="<?php echo return_url().'videos/training/tutorials/'.$video.'.mp4'; ?>"></source>
								</video> -->
							</div>
						</div>
						</a>
					<?php }?>
					<?php } ?>

					</div>
			</div>
		</div>

		<?php
	}

}

?>