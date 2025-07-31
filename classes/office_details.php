<?php
include "Db.php";
include "statics.inc";

$type = $_POST['type'];
$value = $_POST['value'];
$target = $_POST['target'];
$db = new Db();

if($type == ""){
	echo "<option value=''>Empty</option>";
}else if($type == "office"){
	
	$select = $db->select("SELECT dept_id, dept_name FROM department WHERE dept_name IS NOT NULL AND dept_office_id = '$value' AND dept_status = 1 ORDER BY dept_name ASC");
	
	if($db->num_rows()){
		echo '<option value=""> -- Select -- </option>';
		foreach($select[0] as $row){
			extract($row);		
			echo '<option value="'.$dept_id.'">'.$dept_name.'</option>';		
		}
	}else{
		echo "<option value=''> No $target for this $type</option>";
	}
}else if($type == "department"){

	$select = $db->select("SELECT section_id, section_name FROM section WHERE section_name IS NOT NULL AND section_status = 1 AND section_dept_id = '$value' ORDER BY section_name ASC");
	
	
	if($db->num_rows()){
		echo '<option value=""> -- Select -- </option>';
		foreach($select[0] as $row){
			extract($row);
			echo '<option value="'.$section_id.'">'.$section_name.'</option>';
		}
	}else{
		echo "<option value=''> No $target for this $type</option>";
	}
}else if($type == "territory12"){

	$select = $db->select("SELECT territory_id, territory_name FROM territory WHERE territory_name IS NOT NULL AND territory_status = 1 AND territory_office = '$value' ORDER BY territory_name ASC");
	
	
	if($db->num_rows()){
		echo '<option value=""> -- Select -- </option>';
		foreach($select[0] as $row){
			extract($row);
			echo '<option value="'.$territory_id.'">'.$territory_name.'</option>';
		}
	}else{
		echo "<option value=''> No $target for this $type</option>";
	}
}else if($type == "territory"){

	$select = $db->select("SELECT area_office_id, area_office_name FROM area_office WHERE area_office_name IS NOT NULL AND area_office_status = 1 AND area_office_territory_id = '$value' ORDER BY  area_office_name ASC");													
	
	if($db->num_rows()){
		echo '<option value=""> -- Select -- </option>';
		foreach($select[0] as $row){
			extract($row);			
			echo '<option value="'.$area_office_id.'">'.$area_office_name.'</option>';
		}
	}else{
		echo "<option value=''> No $target for this $type</option>";
	}
}elseif ($type == "departmentRoles") {

	$hod = static_hod_id();
	$db = new Db();

	$select = $db->select("SELECT user_designation FROM sysuser WHERE user_designation = '$hod' AND user_department_id = '$value' AND user_active = 1;");
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
		echo '<option value=""> -- Select -- </option>';
		foreach($select[0] as $row){
		extract($row);
			if(in_array($designation_id, $all_hods) ){

			}else{
				echo '<option value="'.$designation_id.'">'.$designation_name.'</option>';
			}
		}
	}else{
		echo "<option value=''> No $target for this $type</option>";
	}


}

echo $db->error();

