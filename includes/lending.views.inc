<?php

/**
 * @file
 *
 * Provide views access into the checkouts data
 *
 */

/**
 * Implementation of hook_views_data
 */
function lending_views_data() {

  drupal_set_message("Loaded lending views");

  $data['lending_checkouts']['table']['group'] = t('Lending Checkouts');

  // Define this as a base table. In reality this is not very useful for
  // this table, as it isn't really a distinct object of its own, but
  // it makes a good example.
  $data['lending_checkouts']['table']['base'] = array(
    'field' => 'cid',
    'title' => t('Lending Checkouts table'),
    'help' => t("Example table contains example content and can be related to nodes."),
    'weight' => -12
  );

  $data['lending_checkouts']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'created_lid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'returned_lid',
    ),
  );

  // This table references the {node} table.
  // This creates an 'implicit' relationship to the node table, so that when 'Node'
  // is the base table, the fields are automatically available.
  /* $data['lending_checkouts']['table']['join'] = array( */
  /*   // Index this array by the table name to which this table refers. */
  /*   // 'left_field' is the primary key in the referenced table. */
  /*   // 'field' is the foreign key in this table. */
  /*   'node' => array( */
  /*     'left_field' => 'nid', */
  /*     'field' => 'nid', */
  /*   ), */
  /* ); */

  // Next, describe each of the individual fields in this table to Views. For
  // each field, you may define what field, sort, argument, and/or filter
  // handlers it supports. This will determine where in the Views interface you
  // may use the field.

  // Node ID field.
  _lending_define_view_number($data, 'lending_checkouts', 'cid', 'Checkout id', 'Unique id associated with the checkout');

  _lending_define_view_date($data, 'lending_checkouts', 'created_at', 'Checked out at', 'When the item was checked out');

  _lending_define_view_date($data, 'lending_checkouts', 'returned_at', 'Returned at', 'When the item was checked back in');


  _lending_define_view_reference(
    $data,
    'lending_checkouts',
    'nid',
    'Node Id',
    'Node that was checked out',
    'node',
    'nid'
  );

  _lending_define_view_reference(
    $data,
    'lending_checkouts',
    'uid',
    'User Id',
    'User that checked out this item',
    'users',
    'uid'
  );

  _lending_define_view_reference(
    $data,
    'lending_checkouts',
    'created_lid',
    'Librarian Id ',
    'Librarian that checked out this item',
    'users',
    'uid'
  );


  return $data;
}


function _lending_define_view_date(&$data, $table, $column, $title, $help = '') {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
     ),
    'filter' => array('handler' => 'views_handler_filter_string_compare'),
    // Amazon date is sortable, so we can just use the standard handler.
    'sort' => array('handler' => 'views_handler_sort'),
  );
}

function _lending_define_view_reference(&$data, $table, $column, $title, $help, $base, $field) {
    $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_numeric',
    ),
    // Information for displaying a title as a field

    'relationship' => array(
      'base' => $base,
      'field' => $field,
      'handler' => 'views_handler_relationship',
      'label' => $title,
    ),
  );

}

/**
 * utility function borrowed from the amazon module
 */
function _lending_define_view_number(&$data, $table, $column, $title, $help) {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
}