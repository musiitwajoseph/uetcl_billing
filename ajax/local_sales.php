<?php 


$factory = $_POST['factory'];

error_reporting(null);
include "../classes/Db.php";
include "../classes/BeforeAndAfter.inc";
$t = new BeforeAndAfter();

$db = new Db();

$select = $db->select("SELECT name FROM valid_names WHERE valid_name_id = '$factory' ");
extract($select[0][0]);

$select = $db->select("SELECT * FROM invoice WHERE inv_from = '$factory' AND (inv_consignment_number IS NULL) AND inv_type IS NULL ORDER BY inv_number ASC");
//echo $db->num_rows();
echo '<h5>'.$name.' ('.$db->num_rows().') &nbsp; &nbsp; <input type="text" value="" placeholder="Search Invoice Number" autocomplete="off" id="searchTerm" style="background-color:#f5ffcf" name="search" onkeyup="doSearch()" onClick="this.select();"/></h5>';
echo '<table id="table" border="1">';
echo '<tr>';
echo '<th>Invoice Number</th>';
echo '<th>Grade</th>';
echo '<th>Kgs</th>';
echo '<th>Bags</th>';
echo '<th>Ugx/Kg</th>';
echo '<th>Net Price</th>';
echo '<th>Sales Tax</th>';
echo '<th>Total Ugx</th>';
echo '<th></th>';
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

	echo '<input type="text" autocomplete="off" class="number" name="" value="" id="aa'.$i.'" onkeyup="return allow(\''.$i.'\');"/>';
	echo '<input type="hidden" value="'.$inv_id.'" id="a'.$i.'"/>';
	echo '<input type="hidden" value="0" id="t'.$i.'"/>';
	echo '<input type="hidden" value="'.$inv_kgs.'" id="kgs'.$i.'"/>';
	echo '<input type="hidden" value="'.$inv_bag_number.'" id="bags'.$i.'"/>';
	echo '</td>';
	echo '<td align="right"><span id="netPrice'.$i.'">0</span></td>';
	echo '<td align="right"><span id="salesTax'.$i.'">0</span></td>';
	echo '<td align="right"><span id="totalWithTax'.$i.'">0</span></td>';
	echo '<td align="right"><span id="sold'.$i.'">Unsold</span></td>';


	echo '</tr>';
	$i++;
}

echo '</table>';

if(!$db->num_rows()) echo '<center><br/><b>No Invoices</b></center>';

echo '<input type="hidden" value="'.($i-1).'" id="allFields"/>';

?>
<script type="text/javascript">
	function doSearch() {
    var searchText = document.getElementById('searchTerm').value;
	searchText = searchText.toUpperCase();
    var targetTable = document.getElementById('table');
    var targetTableColCount;
    //Loop through table rows
    for (var rowIndex = 0; rowIndex < targetTable.rows.length; rowIndex++) {
        var rowData = '';

        //Get column count from header row
        if (rowIndex == 0) {
           targetTableColCount = targetTable.rows.item(rowIndex).cells.length;
           continue; //do not execute further code for header row.
        }
                
        //Process data rows. (rowIndex >= 1)
        for (var colIndex = 0; colIndex < targetTableColCount; colIndex++) {
            rowData += targetTable.rows.item(rowIndex).cells.item(colIndex).textContent;
        }

        //If search term is not found in row data
        //then hide the row, else show
        if (rowData.indexOf(searchText) == -1)
            targetTable.rows.item(rowIndex).style.display = 'none';
        else
            targetTable.rows.item(rowIndex).style.display = 'table-row';
    }
}
</script>
<?php


