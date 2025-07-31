<?php
Class KplcTariff extends BeforeAndAfter{

	public $page = "KPLC";
	
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
	
	public function deleteTariffScheduleAction(){
		$id = portion(3);
		$this->deletor("kplc_tariff_schedule", "ts_id",  $id, 'kplc-tariff/all-tariff-schedule');
	}
	
	public function getLinks(){
		$page = "KPLC";
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
	
	public function addTariffScheduleAction(){

		$date = strtotime(date('Y-m-24'));
		$date = strtotime('-1 months', $date);

		if(!isset($_POST['year'])){
			$year = date('Y', $date);
			$month = date('m', $date);
		}


		echo"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'kplc-tariff/all-tariff-schedule'."><i class='fa fa-eye'></i> View Tariff Schedule</a>";
		if(isset($_POST['addTariffSceduleBtn'])){
		
			$year = trim($_POST['year']);
			$month = trim($_POST['month']);
			$fixed_component = trim($_POST['fixed_component']);
			$escalatable_component = trim($_POST['escalatable_component']);
			$cpim_3 = trim($_POST['cpim_3']);
			$cpib = trim($_POST['cpib']);
			$errors = array();
			$time = time();
			$user = user_id();
			
			
			$errors = array();

			if(empty($year)){
				$errors[] = "Enter Year";
			}
			if(empty($month)){
				$errors[] = "Enter Month";
			}
			if(empty($fixed_component)){
				$errors[] = "Enter Fixed Component";
			}
			if(empty($escalatable_component)){
				$errors[] = "Enter Escalatable Component";
			}
			if(empty($cpim_3)){
				$errors[] = "Enter CPIm-3";
			}
			if(empty($cpib)){
				$errors[] = "Enter CPIb";
			}

			
				
			if(empty($errors)){
				$db = new Db();
				$x = $db->insert("kplc_tariff_schedule",["ts_date_added"=>$time, "ts_year"=>$year, "ts_month"=>$month,"ts_fixed_component"=>$fixed_component,"ts_added_by"=>$user,"ts_escalatable_component"=>$escalatable_component,"ts_cpim_3"=>$cpim_3,"ts_cpib"=>$cpib]);
				echo $db->error();
				
				if($x){
					FeedBack::success();
					FeedBack::refresh(3, return_url().'kplc-tariff/Add-tariff-schedule');
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
					Add Tariff Schedule
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div id="must">All fields with asterisk(*) are mandatory.</div>
								<div class="row">
									<div class="col-md-2">
										<div class="form-group">
											<label>Year<span class="must">*</span></label>
											<div class="form-line">
												
												<?php

												    echo '<select class="form-control" name="year" style="">';
												       echo '<option value="">----select-----</option>';
													    for($i=date('Y')-2; $i<= date('Y')+2; $i++){
													      if($year == $i)
													        echo '<option value="'.$i.'" selected>'.$i.'</option>';
													      else
													        echo '<option value="'.$i.'">'.$i.'</option>';
													    }
												    echo '</select>';
												?>
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Month<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control" name="month">
													<?php
													 echo '<option value="">----select-----</option>';
													    for($i =1; $i<= 12; $i++){
													      if($month == $i)
													        echo '<option value="'.$i.'" selected>'.month_name($i).'</option>';
													      else
													        echo '<option value="'.$i.'">'.month_name($i).'</option>';
													    }	
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Fixed Component<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="number" value="0.09" step="any" name="fixed_component">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Escalatable Component<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="number" value="0.01" step="any" name="escalatable_component">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>CPIm-3<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="number" step="any" name="cpim_3">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>CPIb<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="number" step = "any" name="cpib">
											</div>
										</div>
									</div>
									<div class="col-md-12">
										<div class="form-group">
											<button name="addTariffSceduleBtn" type="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
										</div>
									</div>

								
								</div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
		<script type="text/javascript">
		$(document).off('click','#addTariffSceduleBtn').on('click', '#addTariffSceduleBtn', function(e){
		var year = $('#year').val();
		var month = $('#month').val();
		var fixed_component = $('#fixed_component').val();
		var escalatable_component = $('#escalatable_component').val();
		var cpim_3 = $('#cpim_3').val();
		var cpib = $('#cpib').val();
		
		
		var errors = new Array();

		if(year==""){
			errors.push("Please enter year");
		}

		if(month==""){
			errors.push("Please enter month");
		}
		if(fixed_component==""){
			errors.push("Enter fixed component");
		}
		if(escalatable_component==""){
			errors.push("Enter escalatable component");
		}
		if(cpim_3==""){
			errors.push("Enter CPIm-3");
		}
		if(cpib==""){
			errors.push("Enter CPIb");
		}
		
		
		if(errors.length >= 1){
			alert("Please Review the following:\n "+errors.join("\n"));
		}else{
			//$('#addClassSatusStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');
			$(this).html("Saving");
			$(this).attr("disabled", true);
			var form_data = new FormData();
			form_data.append('year', year);
			form_data.append('month', month);
			form_data.append('fixed_component', fixed_component);
			form_data.append('escalatable_component', escalatable_component);
			form_data.append('cpim_3', cpim_3);
			form_data.append('cpib', cpib);
		

			$.ajax({
			url: "<?php echo return_url().'classes/addTariffSchedule.php';?>",
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				 alert(data);
				var	values = JSON.parse(data);
				if (values.message=='Error') {
					$('#addTariffSceduleBtn').html("Not added");
					$('#addTariffSceduleBtn').attr('disabled',false);
					$('#addTariffSceduleStatus').html('Not Added');
				}else{
					location.reload();
					$('#addTariffSceduleStatus').html('Successfully Saved');
				}
			}
		
		});	
		}

	});
	</script>
	<?php
	}

	
	public function AllTariffScheduleAction(){
	$access = new AccessRights();

	echo"&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'kplc-tariff/add-tariff-schedule'."><i class='fa fa-plus'></i> Add Tariff Schedule</a>";

	echo"&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'kplc-tariff/tariffList'."><i class='fa fa-list'></i> Tariff List</a>";


	if(!$_POST['year'] || !$_POST['month']){
		$year = date('Y');
		$month = date('m');
	}

	if(isset($_POST['addTariffSceduleBtn'])){
		$year = $_POST['year'];
		$month = $_POST['month'];
	}
	?>
	<form action="" method="post">
<div class="row">
<div class="col-md-2">
	<div class="form-group">
		<label>Year<span class="must">*</span></label>
		<div class="form-line">
			
			<?php

			    echo '<select class="form-control" name="year" style="">';
			       echo '<option value="">----select-----</option>';
				    for($i=date('Y')-2; $i<= date('Y')+2; $i++){
				      if($year == $i)
				        echo '<option value="'.$i.'" selected>'.$i.'</option>';
				      else
				        echo '<option value="'.$i.'">'.$i.'</option>';
				    }
			    echo '</select>';
			?>
		</div>
	</div>
</div>
<div class="col-md-2">
	<div class="form-group">
		<label>Month<span class="must">*</span></label>
		<div class="form-line">
			<select class="form-control" name="month">
				<?php
				 echo '<option value="">----select-----</option>';
				    for($i =1; $i<= 12; $i++){
				      if($month == $i)
				        echo '<option value="'.$i.'" selected>'.month_name($i).'</option>';
				      else
				        echo '<option value="'.$i.'">'.month_name($i).'</option>';
				    }	
				?>
			</select>
		</div>
	</div>
</div>
<div class="col-md-3">
	<label><span class="must">&nbsp;</span></label>
	<div class="form-group">
		<button name="addTariffSceduleBtn" type="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Show</button>
	</div>
</div>
</div>
</form>
<?php

echo '<a class="btn btn-success btn-xs" href="" onclick ="return print(\'printMe2\'); return false;"><i class="fa fa-print fa-fw"></i> Print KPLC Tariff</a><Br/><Br/>';

	echo '<style>.td-main{padding:10px;}</style>';
	echo '<style>.td-main-v{padding:10px;}</style>';

	echo '<div id="printMe">';

	//$this->start_print("");

	echo '<table style="font-family:arial; font-size:12px; width:90%;" cellspacing="0" cellpadding="10">';

	echo '<tr><td class="td-main" style="font-weight:bold;border:2px solid #000;">';
	echo 'KPLC TARIFF METHODOLOGY (NEW ENERGY EXCHANGE AGREEMENT -Effective July 2019)';
	echo '</td></tr>';

	echo '<tr><td class="td-main" style="border:2px solid #000;">';
	echo '<b>(EPm) = 0.09+0.01*(CPIm-3/CPIb)</b><br/>';
	echo '<b>Where</b><Br/>';
	echo '<ul>';
	echo '<li>Epm means Energy price per month.</li>';
	echo '<li>0.09  is a fixed component per kWh.</li>';
	echo '<li>0.01 (10%) is the allowable percentage escalable component of the total movement in the CPI.</li>';
	echo '<li>CPIm-3   is the CPI for the 3rd month prior to the beginning of the month to be billed.</li>';
	echo '<li>CPIb – is the Base CPI which is for the month of November 2018 and will remain constant for now.</li>';
	echo '</ul>';
	echo '<Br/>';

	echo '<table cellpadding="5" cellspacing="0" style="font-family:arial; font-size:12px; width:100%";" border="1">';
	echo '<tr>';
	echo '<th class="td-main-v">Month</th>';
	echo '<th class="td-main-v" style="text-align:center;">Fixed <br/>Component/kWh <br/>90%</th>';
	echo '<th class="td-main-v" style="text-align:center;">Escalatable <br/>Component/kWh <br/>10%</th>';
	echo '<th class="td-main-v" style="text-align:center;" colspan="2">CPIm-3</th>';
	echo '<th class="td-main-v" style="text-align:center;">CPIb</th>';
	echo '<th class="td-main-v" style="text-align:center;">Applicable<br/>Tariff (KPLC)</th>';
	echo '</tr>';


	$target2 = $target = strtotime($year.'-'.$month.'-24');

	$start2 = $start = strtotime('-6 months', $target);
	$i=1;
	while($start <= $target2)
	{	
		$ts_escalatable_component = $ts_fixed_component = $ts_cpim_3 = $ts_cpib = "";
		$db = new Db();
		$m = (int)date('m',$start);
		$sql = "SELECT * FROM kplc_tariff_schedule WHERE ts_year = '".date('Y', $start)."' AND ts_month = '".$m."'";
		$select = $db->select($sql);
		extract($select[0][0]);

		$cpi = strtotime('-3 months', $start);
		$tt = ($ts_fixed_component+$ts_escalatable_component*$ts_cpim_3/$ts_cpib);
		$tt = (is_nan($tt))?"":number_format($tt,10);
		echo '<tr>';
		echo '<td class="td-main-v">'.date("M 'y",$start).'</td>';
		echo '<td class="td-main-v">'.number_format($ts_fixed_component,3).'</td>';
		echo '<td class="td-main-v">'.number_format($ts_escalatable_component,3).'</td>';
		echo '<td class="td-main-v">CPI for '.date("M 'y", $cpi).'</td>';
		echo '<td class="td-main-v">'.number_format($ts_cpim_3,3).'</td>';
		echo '<td class="td-main-v">'.number_format($ts_cpib,3).'</td>';
		echo '<td class="td-main-v">'.$tt.'</td>';
		echo '</tr>';
		$start = strtotime('+'.$i.' months', $start2); 
		$i++;
	}

	echo '</table>';
	echo '<br/>';
	echo 'REF : Website for CPIm-3 below';
	echo '<br/>';
	echo '<a href="https://www.bls.gov/regions/new-england/data/consumerpriceindex_us_table.htm" target="_blank">https://www.bls.gov/regions/new-england/data/consumerpriceindex_us_table.htm</a>';

	echo '</td></tr>';

	echo '</table>';

	echo '</div>';

	//$this->end_print();
		
	}
	
	
	public function tariffListAction(){
	$access = new AccessRights();

	echo"&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'kplc-tariff/add-tariff-schedule'."><i class='fa fa-plus'></i> Add Tariff Schedule</a>";

	echo"&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'kplc-tariff/all-tariff-schedule'."><i class='fa fa-list'></i> Tariff Schedule</a>";


	
?>
		<div class="col-md-12">
			<h3>Tariff Schedule List</h3>
			<?php 
			$db = new Db();
			$select = $db->select("SELECT * FROM kplc_tariff_schedule order by ts_date_added desc");
			
			if(!$select){
				$db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr>';
				echo '<th width="30px">No.</th>';
				echo '<th>Year</th>';
				echo '<th>Fixed Component</th>';
				echo '<th>Escalatable Component</th>';
				echo '<th>CPIm-3</th>';
				echo '<th>CPIb</th>';

				//if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
					echo '<th width="100px">Action</th>';
				///}
				echo '</tr>';
				echo '</thead>';
				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					echo '<tr>';
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.$ts_month.'-'.$ts_year.'</td>';
					echo '<td>'.($ts_fixed_component).'</td>';					
					echo '<td>'.($ts_escalatable_component).'</td>';
					echo '<td>'.($ts_cpim_3).'</td>';
					echo '<td>'.($ts_cpib).'</td>';
					
					if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
						echo '<td>';
						if($access->sectionAccess(user_id(), $this->page, 'E'))
							echo"<a class='eagle-load1 btn btn-success btn-xs' href=".return_url().'kplc-tariff/edit-tariff-schedule/'.$ts_id.">Edit</a> &nbsp;&nbsp;&nbsp;";
							//echo $this->action('edit','kplc/edit-tariff-schedule/'.$ts_id, 'Edit');
						if($access->sectionAccess(user_id(), $this->page, 'E'))
							//echo $this->action('delete','kplc/delete-tariff-schedule/'.$ts_id, 'Delete');
							echo"<a class='eagle-load1 btn btn-danger btn-xs' href=".return_url().'kplc-tariff/delete-tariff-schedule/'.$ts_id.">Delete</a>";
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
	
	public function editTariffScheduleAction(){
		$id = portion(3);
		$db=new Db();
		$select=$db->select("SELECT * from kplc_tariff_schedule WHERE ts_id='$id'");
		if($db->num_rows()){
			extract($select[0][0]);
			//print_r($select);
		}
		
		echo"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a class='eagle-load btn-xs btn-primary' href=".return_url().'kplc-tariff/all-tariff-schedule'."><i class='fa fa-eye'></i> View Tariff Schedule</a>";
		if(isset($_POST['addTariffSceduleBtn'])){
		
			$year = trim($_POST['year']);
			$month = trim($_POST['month']);
			$fixed_component = trim($_POST['fixed_component']);
			$escalatable_component = trim($_POST['escalatable_component']);
			$cpim_3 = trim($_POST['cpim_3']);
			$cpib = trim($_POST['cpib']);
			$errors = array();
			$time = time();
			$user = user_id();
			
			
			$errors = array();

			if(empty($year)){
				$errors[] = "Enter Year";
			}
			if(empty($month)){
				$errors[] = "Enter Month";
			}
			if(empty($fixed_component)){
				$errors[] = "Enter Fixed Component";
			}
			if(empty($escalatable_component)){
				$errors[] = "Enter Escalatable Component";
			}
			if(empty($cpim_3)){
				$errors[] = "Enter CPIm-3";
			}
			if(empty($cpib)){
				$errors[] = "Enter CPIb";
			}

			
				
			if(empty($errors)){
				$db = new Db();
				$x = $db->update("kplc_tariff_schedule",["ts_date_added"=>$time, "ts_year"=>$year, "ts_month"=>$month,"ts_fixed_component"=>$fixed_component,"ts_added_by"=>$user,"ts_escalatable_component"=>$escalatable_component,"ts_cpim_3"=>$cpim_3,"ts_cpib"=>$cpib],["ts_id"=>$id]);
				echo $db->error();
				
				if($x){
					FeedBack::success();
					FeedBack::refresh(2, return_url().'kplc-tariff/edit-tariff-schedule/'.$ts_id);
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
					Edit Tariff Schedule
				</div>
				<div class="panel-body">
					<div class="row">
						<div class="col-lg-12">
							<form role="form" action="" method="post">
								<div id="must">All fields with asterisk(*) are mandatory.</div>
								<div class="row">
										<div class="col-md-2">
										<div class="form-group">
											<label>Year<span class="must">*</span></label>
											<div class="form-line">
												
												<?php

												    echo '<select class="form-control" name="year" style="">';
												       echo '<option value="">----select-----</option>';
													    for($i=date('Y')-2; $i<= date('Y')+2; $i++){
													      if($ts_year == $i)
													        echo '<option value="'.$i.'" selected>'.$i.'</option>';
													      else
													        echo '<option value="'.$i.'">'.$i.'</option>';
													    }
												    echo '</select>';
												?>
											</div>
										</div>
									</div>
									<div class="col-md-2">
										<div class="form-group">
											<label>Month<span class="must">*</span></label>
											<div class="form-line">
												<select class="form-control" name="month">
													<?php
													 echo '<option value="">----select-----</option>';
													    for($i =1; $i<= 12; $i++){
													      if($ts_month == $i)
													        echo '<option value="'.$i.'" selected>'.month_name($i).'</option>';
													      else
													        echo '<option value="'.$i.'">'.month_name($i).'</option>';
													    }	
													?>
												</select>
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Fixed Component<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="fixed_component" value="<?php echo $ts_fixed_component;?>">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>Escalatable Component<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="escalatable_component" value="<?php echo $ts_escalatable_component;?>">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>CPIm-3<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="cpim_3" value="<?php echo $ts_cpim_3;?>">
											</div>
										</div>
									</div>
									<div class="col-md-3">
										<div class="form-group">
											<label>CPIb<span class="must">*</span></label>
											<div class="form-line">
												<input class="form-control" type="text" name="cpib" value="<?php echo $ts_cpib;?>">
											</div>
										</div>
									</div>
									<div class="col-md-12">
										<div class="form-group">
											<button name="addTariffSceduleBtn" type="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
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