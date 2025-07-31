<?php
Class Branches extends BeforeAndAfter{
	public $page = "OFFICE";
	public function deleteBranchAction(){		
		$id = portion(3);
		$this->deletor("branch", "branch_id",  $id, 'branches/all-branches');
	}

	public function deleteSectionAction(){
		$id = portion(3);
		$this->deletor("section", "section_id",  $id, 'department/all-sections');
	}
	
	public function getLinks(){
		$page = "OFFICE";
		$links = array(
			array(
				"link_name"=>"Add Office", 
				"link_address"=>"branches/add-branch",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"A",
			),
			array(
				"link_name"=>"View Offices", 
				"link_address"=>"branches/all-branches",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}
	
	public function AddBranch(){
		if(isset($_POST['submit'])){
		
			$branch = $_POST['branch'];
			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();

			if(empty($branch)){
				$errors[] = "Enter Office name";
			}

			if($this->isThere("branch", ["branch_name"=>$branch, "branch_status"=>1])){
				$errors[] = "$branch already exists";
			}
			
			if(empty($errors)){

			$x = $db->insert("branch",["branch_date_added"=>$time, "branch_name"=>$branch,"branch_added_by"=>$user]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'branches/all-branches');
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
					Add Office Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
									<label>Office Name<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" name="branch" placeholder="Enter Office Name" value="<?php echo @$branch; ?>">
									</div>
								</div>
								
								<button onclick = "return confirm('Are you sure you want to save that office ?');" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}
	public function EditBranch(){
		$id=portion(3);
		$db=new Db();
		$select=$db->select("select * from branch WHERE branch_id='$id'");
		extract($select[0][0]);
		if(isset($_POST['submit'])){
		
			$branch = $_POST['branch'];
			$status = $_POST['status'];
			$time = time();
			$user = user_id();
			$db = new Db();
			
			$errors = array();

			if(empty($branch)){
				$errors[] = "Enter Office name";
			}

			if($this->isThereEdit("branch", ["branch_name"=>$branch, "branch_id"=>$id])){
				$errors[] = "$branch already exists";
			}
			
			if(empty($errors)){

			$x = $db->update("branch",["branch_date_added"=>$time, "branch_name"=>$branch,"branch_added_by"=>$user,"branch_status"=>$status],["branch_id"=>$id]);
			
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'branches/all-branches');
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
					Edit Office Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<div id="must">All field with asterisk(*) are mandatory.</div>
									<label>Office Name<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" name="branch" placeholder="Enter Office Name" value="<?php echo @$branch_name; ?>">
									</div>
								</div>
								<div class="clearfix"></div>
								<div>
									<div class="form-group">
								
										<label>Status:<span class="must"></span></label>
										<div class="form-line">
											<?php
											echo '<select name="status">';

											if($branch_status == 0)
												echo '<option value="0" selected="selected">'.$this->show2.'</option>';
											else
												echo '<option value="0">'.$this->show2.'</option>';

											if($branch_status == 1)
												echo '<option value="1" selected="selected">'.$this->show1.'</option>';
											else
												echo '<option value="1">'.$this->show1.'</option>';
											echo '</select>';
											?>
										</div>
									</div>
								</div>
								<button onclick = "return confirm('Are you sure you want to save ?');" type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}
	
	public function AllBranches(){
	$access = new AccessRights();
	?>
		<div class="col-md-12">
			<h3>Office List</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM branch");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Office Name</th>';
				echo '<th>Total Staff</th>';
				echo '<th>Status</th>';
				if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="100px">Action</th>';
				}
				echo '</tr>';
				echo '</thead>';
				
				////////////////////////////REPORT STEP 1//////////////////
				$db_values = array();
				$db_values[] = array(
				
					
					"No",
					"Office Name",
					"Total Staff"
								);
				//////////////////////////////////////////////////////////////
				
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.($branch_name).'</td>';
					echo '<td>'.number_format($this->total("sysuser", "user_branch_id", $branch_id)).'</td>';
					echo '<td>';
					if(empty($branch_status)){
						echo $this->show22;
					}else{
						echo $this->show11;
					}
					echo '</td>';
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					
						echo '<td>';
						if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo $this->action('edit','branches/edit-branch/'.$branch_id, 'Edit');
						if($access->sectionAccess(user_id(), $this->page, 'D'))
							echo $this->action('delete','branches/delete-branch/'.$branch_id, 'Delete');
						echo '</td>';
					}
					echo '</tr>';
					
					//////////////////////////////////REPORT STEP 2//////////////////////////////////	
					$db_values[] = array(
					
						($i-1),
						($branch_name),
						number_format($this->total("sysuser", "user_branch_id", $branch_id))
					
					); 
					
					/////////////////////////////////////////////////////////////////////////////
					
				}
				echo '</tbody>';
				
				echo '</table>';
				
				//////////////////////////////////////////REPORT STEP 3/////////////////////////////
				$t = new TableCreator();
				$heading = "OFFICES LIST"; //CHANGE
				$t->open($this->full_name(user_id()), $heading);
				$t->thd($db_values);
				$t->close();
				$t->results();

				$e = new Exporter();
				echo $e->getDisplay($heading, $t->results());
				/////////////////////////////////////////////////////////////////////////////
				
			}
			
			?>
		</div>
		<?php
	}
	
	public function EditDepartmentAction(){
		$id = portion(3);
		$db = new Db();
		$select = $db->select("SELECT * FROM department WHERE dept_id = '$id'");
		
		extract($select[0][0]);
		
		$department = $dept_name;
		if(isset($_POST['submit'])){
		
			$department = $_POST['department'];
			$time = time();
			$user = user_id();
			$db = new Db();
			$time = time();
			
			$x = $db->update("department", ["dept_name"=>$department, "dept_last_changed_date"=>$time], ['dept_id'=>$id]);
			
			if(empty($db->error())){
				FeedBack::success();
				//FeedBack::refresh(2, return_url()."department/all-departments");
			}else{
				FeedBack::error("An error occured.");
			}
		}
		?>
		<div class="col-md-4">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Edit Department Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<label>Department Name</label>
									<div class="form-line">
										<input class="form-control" type="text" name="department" placeholder="Enter Department Name" value="<?php echo @$department; ?>">
									</div>
								</div>
								
								<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}
	
	
	public function AddSectionAction(){
		if(isset($_POST['submit'])){
		
			$section = $_POST['section_name'];
			$department_id = $_POST['department_id'];
			$time = time();
			$user = user_id();
			$db = new Db();
		
			$x = $db->insert("section",["section_date_added"=>$time,"section_dept_id"=>$department_id, "section_name"=>$section,"section_added_by"=>$user]);
			echo $db->error();
			if($x){
				FeedBack::success();
				FeedBack::refresh(3, return_url().'department/all-sections');
			}else{
				FeedBack::error();
			}
		}
		?>
		<div class="col-md-4">
			<h3>&nbsp;</h3>
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-heading">
					Add Section Form
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div class="form-group">
									<label>Section Name</label>
									<div class="form-line">
										<input class="form-control" type="text" name="section_name" placeholder="Enter Department Name" value="<?php echo @$department; ?>">
									</div>
								</div>
								<div class="form-group">
								
									<label>Belongs To:</label>
									<div class="form-line">
										<select name="department_id">
											<option value="">--Select--</option>
											<?php
											$db = new Db();
											$select = $db->select("SELECT dept_id, dept_name FROM department ORDER BY dept_name ASC");
											foreach($select[0] as $row){
												extract($row);
												if($dept_id == $department_id)
													echo '<option value="'.$dept_id.'" selected="selected">'.$dept_name.'</option>';
												else
													echo '<option value="'.$dept_id.'">'.$dept_name.'</option>';
											}
											?>
										</select>
									</div>
								</div>
								
								<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	<?php
	}
	
	public function AllSectionsAction(){
	
	?>
		<div class="col-md-12">
			<h3>Section List</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM department, section WHERE dept_id = section_dept_id ");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Section Name</th>';
				echo '<th>Department Name</th>';
				echo '<th>Total Staff</th>';
				echo '<th width="100px">Action</th>';
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.($section_name).'</td>';
					echo '<td>'.($dept_name).'</td>';
					echo '<td></td>';
					echo '<td>';
					echo $this->action('edit','department/edit-section/'.$section_id, 'Edit');
					echo $this->action('delete','department/delete-section/'.$section_id, 'Delete');
					echo '</td>';
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