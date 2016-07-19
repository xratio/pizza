<?php
/**
 * Implements pizza_list_form().
 */
function pizza_list_form($form, &$form_state){
  
  $form = array();
  
  // Note: select all record from pizza_type table
  $query = db_select('pizza_type', 'p')->extend('PagerDefault')
      ->fields('p', array('pid', 'piza_name', 'piza_price'))
      ->orderBy('pid', 'DESC')
	  ->limit(20)
      ->execute();
	
  // Select pizza data from database by pid
  if(!empty($_GET['edit'])){
    $pid = $_GET['edit'];
  }elseif(!empty($_GET['delete'])){
     $pid = $_GET['delete'];
  }else{
     $pid = '';
  }
  
  // Note: select one record from database
  $query_single = db_select('pizza_type', 'p')
    ->fields     ('p', array('pid', 'piza_name', 'piza_price'))
    ->condition  ('p.pid', $pid)
    ->orderBy    ('pid', 'DESC')
    ->execute();
	
  while($record    = $query_single->fetchAssoc()) {
    $pizza_name    = $record['piza_name'];
    $pizza_price   = $record['piza_price'];
    $pizza_pid     = $record['pid'];
  }
  
  // Note: Chek if not exist $_GET['edit'] or $_GET['delete'].
  // If not exist show full list of pizza type.
  if(empty($_GET['edit']) && empty($_GET['delete'])){
    
    // Note: Build the sortable table header.
    $header = array(
      'piza_name'    => t('Pizza name'),
      'piza_price'   => t('Pizza price'),
	  'actions'      => t('Actions'),
    );
  
    $rows = array();
    $links = array();
    foreach ($query as $result){
	   $links = array(
         'edit'       => l(t('Edit'), 'admin/config/pizza-region/pizza', array('query' => array('edit' => $result->pid), 'attributes' => array(
		   'class'    => array('edit-link'))
		   )),
           'delete'   => l(t('Delete'), 'admin/config/pizza-region/pizza', array('query' => array('delete' => $result->pid), 'attributes' => array(
		   'class'    => array('delete-link'))
		   )),
        );
      $rows[] = array(
        'piza_name'    => $result->piza_name,
	    'piza_price'   => $result->piza_price,
  	    'actions'      => $links['edit'] . ' / ' . $links['delete'],
	    '#attributes'  => array('class' => array('pizza-rows')),
      );
    }
    
	// Note: Render full list of pizza type
    $form['pizza_table']['table_list'] = array(
      '#type'        => 'tableselect',
	  '#header'      => $header,
	  '#options'     => $rows,
	  '#empty'       => t('No pizza in database...'),
	  '#attributes'  => array('class' => array('pizza')),
	  '#disabled'    => TRUE
    );
  
    $form['pager'] = array('#markup' => theme('pager'));

	return $form;
	
  }elseif(!empty($_GET['edit'])){
	
	if(!isset($_SESSION['messages'])){
	
	$form['name']      = array(
    '#title'           => 'Pizza name',
	'#type'            => 'textfield',
	'#default_value'   => $pizza_name,
	'#required'        => TRUE,
    );
  
    $form['price']       = array(
      '#title'           => 'Pizza price',
	  '#type'            => 'textfield',
	  '#default_value'   => $pizza_price,
	  '#required'        => TRUE,
    );
	
	$form['edit'] = array(
	  '#type'     => 'hidden',
	  '#value'    => $pizza_pid
	);
	
	if($_GET['edit']){
	  $edit = TRUE;	
	}
	$form['edit_status'] = array(
	  '#type'            => 'hidden',
	  '#value'           => $edit
	);
	
	$form['submit'] = array(
	  '#type'       => 'submit',
	  '#value'      => t('Save')
	);
    
	
	//drupal_set_message(print_r($form['submit']));
	return $form;
	
	}else{
      //drupal_set_message($_GET['edit']);
	}
	
  }elseif(!empty($_GET['delete'])){
	$form = array();

	if($_GET['delete']){
	$delete = TRUE;	
	}
	
	$form['delete_status'] = array(
	  '#type'              => 'hidden',
	  '#value'             => $delete
	);
	
	$form['name'] = array(
	  '#markup'   => 'Are you sure you want to delete ' . $pizza_name . ' pizza <br />'
    );

	if(!isset($_SESSION['messages'])){
      $form['submit'] = array(
	    '#type'       => 'submit',
	    '#value'      => t('Delete')
	  );
	}
	
	return $form;
	 
  }else{
	  
    return $form;
	
  }
}

/**
 * Implements pizza_list_form_submit().
 */
function pizza_list_form_submit($form, &$form_state){
  /*if($form_state['values']['edit'] == TRUE)*/
  if(!empty($form_state['values']['edit_status'])){
  $pizza_name   = $form_state['values']['name'];
  $pizza_price  = $form_state['values']['price'];
  
  $query = db_update('pizza_type')
    ->fields(array(
	  'piza_name'   => $pizza_name,
	  'piza_price'  => $pizza_price,
	  ))
    ->condition('pid', $form_state['values']['edit'])
    ->execute();
	
  drupal_set_message($form_state['values']['name'] . ' was edited with success!');
  $form_state['redirect'] = 'admin/config/pizza-region/pizza';
  }else{
	 //Delete pizza data from database
	   $pid    = $_GET['delete'];
       $query  = db_delete('pizza_type')
          ->condition('pid', $pid)
          ->execute();
    drupal_set_message('Item was deleted with success.');
	$form_state['redirect'] = 'admin/config/pizza-region/pizza';
  }
  
}

/**
 * Implements add_pizaa_add_form().
 */
function pizza_add_form($form, &$form_state){
  $form['name'] = array(
    '#title'    => 'Pizza name',
	'#type'     => 'textfield',
	'#required' => TRUE,
  );
  
  $form['price'] = array(
    '#title'     => 'Pizza price',
	'#type'      => 'textfield',
	'#required'  => TRUE,
  );
  
  $form['sibmit'] = array(
    '#type'       => 'submit',
	'#value'      => 'Add pizza',
  );
  
  return $form;
}

/**
 * Implements pizza_add_form_submit().
 */
function pizza_add_form_submit($form, &$form_state){
  $pizza_name  = $form_state['values']['name'];
  $pizza_price = $form_state['values']['price'];
  
  $result = db_insert('pizza_type')
    ->fields(array(
      'piza_name'  => $pizza_name,
      'piza_price' => $pizza_price,
    ))
	->execute();
  
  if($result === FALSE) {
    drupal_set_message(t('Error'), 'error');
  }else{
	drupal_set_message(t($pizza_name . ' успешно добавлена!'));
  }
}


/**
 * Implements pizza_regionlist_form().
 */
function pizza_region_form($form, &$form_state){
  $form = array();
  
  // Note: select all record from pizza_type table
  $query = db_select('pizza_region', 'r')->extend('PagerDefault')
      ->fields('r', array('rid', 'region_name', 'region_delivery_price'))
      ->orderBy('rid', 'DESC')
	  ->limit(20)
      ->execute();
	
  // Initiate $rid value
  if(!empty($_GET['edit'])){
    $rid = $_GET['edit'];
  }elseif(!empty($_GET['delete'])){
     $rid = $_GET['delete'];
  }else{
     $rid = '';
  }
  
  // Note: select one record from database
  $query_single = db_select('pizza_region', 'r')
    ->fields('r', array('rid', 'region_name', 'region_delivery_price'))
    ->condition('r.rid', $rid)
    ->orderBy('rid', 'DESC')
    ->execute();
	
  while($record   = $query_single->fetchAssoc()) {
    $region_name  = $record['region_name'];
    $region_price = $record['region_delivery_price'];
    $region_rid   = $record['rid'];
  }
  
  // Note: Chek if not exist $_GET['edit'] or $_GET['delete'].
  // If not exist show full list of pizza type.
  if(empty($_GET['edit']) && empty($_GET['delete'])){
    
    // Note: Build the sortable table header.
    $header          = array(
      'region_name'  => t('Region name'),
      'region_price' => t('Region price'),
	  'actions'      => t('Actions'),
    );
  
    $rows  = array();
    $links = array();
    foreach ($query as $result){
	   $links = array(
         'edit'     => l(t('Edit'), 'admin/config/pizza-region/region', array('query' => array('edit' => $result->rid), 'attributes' => array(
		   'class'  => array('edit-link'))
		   )),
           'delete' => l(t('Delete'), 'admin/config/pizza-region/region', array('query' => array('delete' => $result->rid), 'attributes' => array(
		   'class'  => array('delete-link'))
		   )),
        );
      $rows[]           = array(
        'region_name'   => $result->region_name,
	    'region_price'  => $result->region_delivery_price,
  	    'actions'       => $links['edit'] . ' / ' . $links['delete'],
	    '#attributes'   => array('class' => array('pizza-rows')),
      );
    }
    
	// Note: Render full list of pizza type
    $form['pizza_table']['table_list'] = array(
      '#type'                          => 'tableselect',
	  '#header'                        => $header,
	  '#options'                       => $rows,
	  '#empty'                         => t('No pizza in database...'),
	  '#attributes'                    => array('class' => array('pizza')),
	  '#disabled'                      => TRUE
    );
  
    $form['pager'] = array('#markup' => theme('pager'));

	return $form;
	
  }elseif(!empty($_GET['edit'])){
	
	if(!isset($_SESSION['messages'])){
	
	$form['name']    = array(
    '#title'         => 'Region name',
	'#type'          => 'textfield',
	'#default_value' => $region_name,
	'#required'      => TRUE,
    );
  
    $form['price']      = array(
      '#title'          => 'Region delivery price',
	  '#type'           => 'textfield',
	  '#default_value'  => $region_price,
	  '#required'       => TRUE,
    );
	
	$form['edit'] = array(
	  '#type'     => 'hidden',
	  '#value'    => $region_rid
	);
	
	if($_GET['edit']){
	  $edit = TRUE;	
	}
	$form['edit_status'] = array(
	  '#type'            => 'hidden',
	  '#value'           => $edit
	);
	
	$form['submit'] = array(
	  '#type'       => 'submit',
	  '#value'      => t('Save')
	);
    
	
	//drupal_set_message(print_r($form['submit']));
	return $form;
	
	}else{
      //drupal_set_message($_GET['edit']);
	}
	
  }elseif(!empty($_GET['delete'])){
	$form = array();

	if($_GET['delete']){
	$delete = TRUE;	
	}
	
	$form['delete_status'] = array(
	  '#type'              => 'hidden',
	  '#value'             => $delete
	);
	
	$form['name'] = array(
	  '#markup' => 'Are you sure you want to delete ' . $region_name . ' region? <br />'
    );

	if(!isset($_SESSION['messages'])){
      $form['submit'] = array(
	    '#type' => 'submit',
	    '#value' => t('Delete')
	  );
	}
	
	return $form;
	 
  }else{
	  
    return $form;
	
  }

}

/**
 * Implements pizza_region_form_submit(().
 */
function pizza_region_form_submit($form, &$form_state){
  if(!empty($form_state['values']['edit_status'])){
    
    $region_name = $form_state['values']['name'];
    $region_price = $form_state['values']['price'];
      
	  // Update region data
      $query = db_update('pizza_region')
        ->fields(array(
    	  'region_name' => $region_name,
    	  'region_delivery_price' => $region_price,
    	  ))
        ->condition('rid', $form_state['values']['edit'])
        ->execute();
	
    drupal_set_message($form_state['values']['name'] . ' was edited with success!');
    $form_state['redirect'] = 'admin/config/pizza-region/region';
	
  }elseif(!empty($form_state['values']['delete_status'])){
    // Delete region data from database
      $rid = $_GET['delete'];
      $query = db_delete('pizza_region')
        ->condition('rid', $rid)
        ->execute();
		
    drupal_set_message('Item was deleted with success.');
	$form_state['redirect'] = 'admin/config/pizza-region/region';
  }else{
	drupal_set_message('Unknown debug.');
  }
}

/**
 * Implements pizza_regionadd_form(().
 */
function pizza_regionadd_form($form, &$form_state){
  $form = array();
  
  $form['name'] = array(
    '#title' => 'Region name',
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  
  $form['delivery_price'] = array(
    '#title' => 'Region delivery price',
	'#type' => 'textfield',
	'#required' => TRUE,
  );
  
  $form['sibmit'] = array(
    '#type' => 'submit',
	'#value' => 'Add region',
  );
  
  return $form;
}

/**
 * Implements pizza_regionadd_form_submit(().
 */
function pizza_regionadd_form_submit($form, &$form_state){
  $region_name = $form_state['values']['name'];
  $region_delivery_price = $form_state['values']['delivery_price'];
  
  $result = db_insert('pizza_region')
    ->fields(array(
      'region_name'           => $region_name,
      'region_delivery_price' => $region_delivery_price,
    ))
	->execute();
  
  if($result === FALSE) {
    drupal_set_message(t('Error'), 'error');
  }else{
	drupal_set_message(t($region_name . ' успешно добавлена!'));
  }
}