<?php 


$factory = $_POST['factory'];

error_reporting(null);
include "../classes/Db.php";
include "../classes/BeforeAndAfter.inc";
$t = new BeforeAndAfter();

$db = new Db();

$select = $db->select("SELECT name FROM valid_names WHERE valid_name_id = '$factory' ");
extract($select[0][0]);

$sql = "SELECT * FROM invoice WHERE inv_from = '$factory' AND inv_type IS NULL AND (inv_consignment_number IS NULL or inv_consignment_number = '') ORDER BY inv_number ";
$select = $db->select($sql);


echo '<h5>'.$name.' ('.number_format($db->num_rows()).')</h5>';
//echo $db->num_rows();
echo '<table id="table" border="1">';
echo '<tr>';
echo '<th>Invoice Number</th>';
echo '<th>Grade</th>';
echo '<th>Kgs</th>';
echo '<th>Bags</th>';
echo '<th>Allocate</th>';
echo '</tr>';
$i = 1;
foreach($select[0] as $row){
	extract($row);
	echo '<tr>';
	echo '<td>'.$inv_number.'</td>';
	echo '<td>'.$t->rgf('tea_grades', $inv_grade, 'tg_id', 'tg_grade').'</td>';
	echo '<td>'.number_format($inv_kgs).'</td>';
	echo '<td>'.number_format($inv_bag_number).'</td>';
	echo '<td>'; 
	echo '<input type="checkbox" name="allocated[]" value="'.$inv_id.'" id="a'.$i.'" onclick="return allow(\''.$i.'\');"/><label for="a'.$i.'" class="toAllocate"></label>';
	echo '<input type="hidden" value="0" id="t'.$i.'"/>';
	echo '<input type="hidden" value="'.$inv_kgs.'" id="kgs'.$i.'"/>';
	echo '<input type="hidden" value="'.$inv_bag_number.'" id="bags'.$i.'"/>';
	echo '</td>';
	echo '</select>';
	echo '</td>';

	echo '</tr>';
	$i++;
}

echo '</table>';

if(!$db->num_rows()) echo '<center><br/><b>No Invoices</b></center>';

echo '<input type="hidden" value="'.($i-1).'" id="allCheckBoxes"/>';


