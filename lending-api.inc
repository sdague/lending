<?php

/******************************************************
 *
 *  Lending API
 *
 *  The following functions are the ways we interact with the
 *  database, and do access controls.  All other functions in lending
 *  should call functions in this file and not embed SQL directly.
 */

define('LEND_ACCESS','access lending library'); // lendee
define('LEND_ADMIN','administer lending library'); // librarian

/**
 *
 */
function lending_checkout_item($nid, $uid, $librarian) {
  if (lending_is_checked_out($nid)) {
    // ERROR
    return;
  }
  return db_query("INSERT INTO {lending_checkouts} (nid, uid, created_lid, created_at) VALUES (%d, %d, %d, %d)", $nid, $uid, $librarian, time());

}

/**
 *
 */
function lending_checkin_item($cid, $librarian) {
  return db_query("UPDATE {lending_checkouts} set returned_at = %d, returned_lid = %d where cid = %d", time(), $librarian, $cid);

}

function lending_current_checkout($nid) {
  $result = db_query("SELECT * from {lending_checkouts} where nid = %d and returned_at = 0", $nid);
  if ($record = db_fetch_array($result)) {
    return $record;
  }
  else {
    return 0;
  }
}

function lending_current_checkout_user($nid) {

  $result = db_query("SELECT uid from {lending_checkouts} where nid = %d and returned_at = 0", $nid);
  if ($user = db_fetch_array($result)) {
    $u = user_load($user['uid']);
    return $u;
  }
  else {
    return 0;
  }
}

function lending_is_checked_out($nid) {
  $count = db_result(db_query("SELECT count(cid) from {lending_checkouts} where nid = %d and returned_at = 0", $nid));
  return ($count > 0);
}


/**
 * Determine that this isn't a valid user to use this function.
 */

function lending_not_admin($user) {
  return ! (user_access(LEND_ADMIN, $user));
}

function lending_is_admin($user) {
  return (user_access(LEND_ADMIN, $user));
}

function lending_is_user_or_admin($user) {
  return (user_access(LEND_ACCESS, $user) or user_access(LEND_ADMIN, $user));
}

function lending_not_user_or_admin($user) {
  return ! (user_access(LEND_ACCESS, $user) or user_access(LEND_ADMIN, $user));
}

function lending_not_acceptable_type($node) {
  $types_to_lend = variable_get('lending_node_types', array());

  // Abort if this node is not one of the types we can lend
  return !in_array($node->type, $types_to_lend);
}
