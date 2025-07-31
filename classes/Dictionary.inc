<?php
Class Dictionary extends BeforeAndAfter{
	public $page = "USERS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public static function getLinks(){
		$page = "USERS";
		$links = array(
		array(
        "link_name"=>"Dictionary", 
        "link_address"=>"dictionary/add-dictionary",
        "link_icon"=>"fa-plus",
        "link_page"=>$page,
        "link_right"=>"V",
      ),
			array(
				"link_name"=>"Dictionary", 
				"link_address"=>"dictionary/dictionary",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			),
      array(
        "link_name"=>"Import Dictionary", 
        "link_address"=>"dictionary/import-dictionary",
        "link_icon"=>"fa-upload",
        "link_page"=>$page,
        "link_right"=>"V",
      ),
			array(
        "link_name"=>"Category", 
        "link_address"=>"dictionary/category",
        "link_icon"=>"fa-eye",
        "link_page"=>$page,
        "link_right"=>"V",
      ),
			
			
		);
		
		return $links;
	}
	


	
	
public function deleteCategoryAction(){
    $id = portion(3);
    $this->deletor("dictionary_category", "id",  $id, 'dictionary/category');
  }
  
  public function CategoryAction(){
    $id2 = portion(3);
    $db = new Db();
    if($id2){
     $select=$db->select("select * from dictionary_category WHERE id='$id2'");
     if($db->num_rows());
     extract($select[0][0]);
     //print_r($select);
    }
    if(isset($_POST['submit'])){
    
      $category = $_POST['category'];
     
      $time = time();
      $user = user_id();
      $db = new Db();
      
      $errors = array();
      
      if(empty($category)){
        $errors[]="Enter category";
      }
     
      
      if(empty($errors)){
          if($id2){
          $db->update("dictionary_category",["dc_date_added"=>$time, "dc_name"=>"$category","dc_added_by"=>$user],["id"=>$id2]);
          }else{
           $db->insert("dictionary_category",["dc_date_added"=>$time, "dc_name"=>"$category","dc_added_by"=>$user]);
          }

        if(!$db->error()){
          FeedBack::success();
          FeedBack::refresh(3, return_url().'dictionary/category');
        }else{
          FeedBack::error();
        }
      }else{
        FeedBack::errors($errors);
      }
    }
    ?>
    <div class="col-md-3">
  
          <div class="row">
            <div class="col-lg-12">
              <form role="form" action="" method="post">
                <div class="form-group">
                
                  <label>Category Name<span class="must">*</span></label>
                  <div class="form-line">
                    <input class="form-control" type="text" name="category" placeholder="Enter category name" value="<?php if($id2) echo$dc_name; else echo @$category; ?>">
                  </div>
                </div>
                
                <button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
              </form>
            </div>
          </div>
        
   </div>
   <div class="col-md-1">
   </div>
   <div class="col-md-7">
       
   
      <?php 
      $access = new AccessRights();
      $db = new Db();
      $select = $db->select("SELECT * FROM dictionary_category ORDER BY dc_name ASC");
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
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>';
          echo '<td>'.($dc_name).'</td>';
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
            if($access->sectionAccess(user_id(), $this->page, 'E'))
              //echo $this->action('edit','dictionary/category/'.$id, 'Edit');
             echo '<a class="btn-xs btn btn-success" href="'.return_url().'dictionary/category/'.$id.'">Edit</a>';
            //if($access->sectionAccess(user_id(), $this->page, 'D'))
              echo $this->action('delete','dictionary/delete-category/'.$id, 'Delete');
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
 public function importDictionaryAction(){
    $db = new Db();
    $time = time();
    $user = user_id();
 
    if (isset($_POST['send'])) {
      $category = $_POST['category'];
        $errors = array();
     
      $filename=$_FILES["file"]["tmp_name"];
      if($_FILES["file"]["size"] > 0){

        $valid_name = "Name";
        $file = fopen($filename, "r");
        while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
          $count++;
          $col_1 = @trim(str_replace("'","\'",strip_tags($emapData[0])));
          $col_2 = @trim(str_replace("'","\'",strip_tags($emapData[1])));
          $col_3 = @trim(str_replace("'","\'",strip_tags($emapData[2])));
       

          if($count == 1){
           //echo "$col_1 is equal to $valid_name";
           if($col_1 != $valid_name){
             $errors[] = "Invalid Template";
           }
          }
          
          
          echo $db->error();
        }
         if(empty($category)){
            $errors[] = "Please Select Category name";
          } 
          
        fclose($file);

        if(empty($errors)){
          $count = 0;
          $db = new Db();
          $file = fopen($filename, "r");
          while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
            $count++;
            if($count > 1){
            $col_1 = @trim(str_replace("'","\'",strip_tags($emapData[0])));
            $col_2 = @trim(str_replace("'","\'",strip_tags($emapData[1])));
            $col_3 = @trim(str_replace("'","\'",strip_tags($emapData[2])));
            
           
            $db = new Db();
            $db->insert("dictionary",["d_name"=>$col_1,"d_description"=>$col_2,"d_code"=>$col_3,"d_date_added"=>$time,"d_added_by"=>$user,"d_category"=>$category]);
          
            
          }

          }

          if(!$db->error()){
            FeedBack::success();
            FeedBack::refresh(3, return_url().'dictionary/import-dictionary');
          }else{
            FeedBack::errors($errors);
          }
        }else{
          FeedBack::errors($errors);
        }
      }else{ 
        $errors[] = "Please Attach file";
      }

    
      }

   echo '<form action="" method="post" enctype="multipart/form-data">';
    
    echo '<div class="col-md-3">';
    echo '<label>Category</label>';
    echo '<select class="form-control" name="category" style="height:;">';
    echo "<option value=''>-----select-----</option>";
    $db = new Db();
      $select = $db->select("SELECT * from dictionary_category");
      foreach ($select[0] as $rows) {
        extract($rows);
          echo'<option value='.$id.'>'.$dc_name.'</option>';
      }
    echo '</select>';
    echo '</div>';
    
    echo '<div class="col-md-3">';
    echo '<label>Attach Dictionary File</label>';
    echo '<input type="file" name="file">';
    echo '</div>';

    echo '<div class="col-md-3">';
    echo '<br/>';
    echo '<button name="send" type="submit"><i class="fa fa-fw fa-plane"></i> Upload</button>';
    echo '</div>';
    echo '</form>';
  
  }
 public function deleteDictionaryAction(){
     $id = portion(3);
    $this->deletor("dictionary", "d_id",  $id, 'dictionary/dictionary');
  }
  
  public function addDictionaryAction(){
    $id2 = portion(3);
    $db = new Db();
    if($id2){
     $select=$db->select("select * from dictionary WHERE d_id='$id2'");
     if($db->num_rows());
     extract($select[0][0]);
    }
    if(isset($_POST['submit'])){
    
      $category = $_POST['category'];
      $name = $_POST['name'];
      $code = $_POST['code'];
      $description = $_POST['description'];
     
      $time = time();
      $user = user_id();
      $db = new Db();
      
      $errors = array();
      
      if(empty($category)){
        $errors[]="Enter category";
      }
      
      
      if(empty($errors)){
          if($id2){
          $db->update("dictionary",["d_name"=>$name,"d_description"=>$description,"d_code"=>$code,"d_date_added"=>$time,"d_added_by"=>$user,"d_category"=>$category],["d_id"=>$id2]);
          }else{
            $db->insert("dictionary",["d_name"=>$name,"d_description"=>$description,"d_code"=>$code,"d_date_added"=>$time,"d_added_by"=>$user,"d_category"=>$category]);
          }

        if(!$db->error()){
          FeedBack::success();
          if($id2){
             FeedBack::refresh(3, return_url().'dictionary/add-dictionary/'.$id2);
          }else{
             FeedBack::refresh(3, return_url().'dictionary/add-dictionary');
          }
         
        }else{
          FeedBack::error();
        }
      }else{
        FeedBack::errors($errors);
      }
    }
    ?>
    <div class="col-md-3">
  
          <div class="row">
            <div class="col-lg-12">
              <form role="form" action="" method="post">
                <div class="form-group">
                 
                    <label>Category Name<span class="must">*</span></label>
                     <div class="form-line">
                    <?php
                     echo '<select class="form-control" name="category" style="height:;">';
                        echo "<option value=''>-----select-----</option>";
                        $db = new Db();
                          $select = $db->select("SELECT * from dictionary_category");
                          foreach ($select[0] as $rows) {
                            extract($rows);
                            if($d_category ==$id)
                              echo'<option selected value='.$id.'>'.$dc_name.'</option>';
                              else
                              echo'<option value='.$id.'>'.$dc_name.'</option>';
                          }
                        echo '</select>';
                    ?>
                  </div>
                  </div>
                  <div class="form-group">
                    <label>Name<span class="must">*</span></label>
                   <div class="form-line">
                     <input type="text" placeholder="Name" value="<?php echo$d_name;?>"  name="name" class="form-control">
                   </div>
                  </div>
                  <div class="form-group">
                    <label>Code<span class="must">*</span></label>
                   <div class="form-line">
                     <input type="text" placeholder="Code" value="<?php echo$d_code;?>" name="code" class="form-control">
                   </div>
                  </div>
                   <div class="form-group">
                    <label>Description<span class="must">*</span></label>
                   <div class="form-line">
                     <textarea class="form-control" name="description" placeholder="Description"><?php echo $d_description;?></textarea>
                   </div>
                  </div>
                  <?php
                    if ($id2) {
                      echo '<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Update</button>';
                    }else{
                       echo'<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>';
                    }
                  ?>
                 
              </form>
            </div>
          </div>
        
   </div>
  <?php 
 } 
 public function dictionaryAction(){
   $access = new AccessRights();
      $db = new Db();
      $select = $db->select("SELECT * FROM dictionary ORDER BY d_name ASC");
      //print_r($select);
      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Name</th>';
        echo '<th>Code</th>';
        echo '<th>Description</th>';
        echo '<th>Category</th>';
        
        if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
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
          echo '<td>'.($d_name).'</td>';
          echo '<td>'.($d_code).'</td>';
          echo '<td>'.($d_description).'</td>';
          echo '<td>'.$this->rgf("dictionary_category",$d_category,"id","dc_name").'</td>';
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
            if($access->sectionAccess(user_id(), $this->page, 'E'))
              //echo $this->action('edit','dictionary/add-dictionary/'.$d_id, 'Edit');
            echo '<a class="btn-xs btn btn-success" href="'.return_url().'dictionary/add-dictionary/'.$d_id.'">Edit</a>';
            //if($access->sectionAccess(user_id(), $this->page, 'D'))
              echo $this->action('delete','dictionary/delete-dictionary/'.$d_id, 'Delete');
            echo '</td>';
          }
          echo '</tr>';
         
        }
        echo '</tbody>';
        
        echo '</table>';
        
      
        
      }
 }
}