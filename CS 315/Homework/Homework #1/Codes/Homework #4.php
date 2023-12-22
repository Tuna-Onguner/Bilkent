<?php
    
/*
 * Bilkent University
 * Fall 2022 - 2023
 * CS315 Programing Languages
 * Homework 1: Associative Arrays
 * "PHP Associative Arrays"
 *
 * 18.11.2022, Friday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://www.geeksforgeeks.org/associative-arrays-in-php/
 * https://www.w3schools.com/php/php_arrays_associative.asp
 */
    
    
    echo "CS315 Homework 1\nPHP Associative Arrays\n";
    
    // Initialize an associaive array
    $populationInMillion = array(
        "Turkey" => 85, 
        "Germany" => 83, 
        "France" => 67.5, 
        "Italy" => 59
    );
    
    // Print the associaive array
    echo "\nPrinting popolation associative array:\n";
    print_r ($populationInMillion);
    
     // Size of the array
    $size = count($populationInMillion);
    echo "\nElement count of the array is " . $size . "\n";
    
    // Adding new elements
    echo "\nBelgium is added\n";
    $populationInMillion["Belgium"] = 11.5;
    
    // Print alternative way
    echo "\nAnother approach to print:\n";
    var_dump ($populationInMillion);

    // Accessing elements
    $popItaly = $populationInMillion["Italy"];
    $popGermany = $populationInMillion['Germany'];
    echo "\nPopulation of Italy is " . $popItaly . 
        " millions\nPopulation pf Germany is " . $popGermany . " millions\n";
        
    // Modifying elements
    $populationInMillion["Turkey"] += 5;
    $populationInMillion["Belgium"] = 12.0;
    
    // Removing elements
    unset ($populationInMillion["France"]);
    
    // Print updated array
    echo "\nUpdated array:\n";
    print_r ($populationInMillion);
    
    // Search in keys
    function searchInKeys ($array, $key) {
        $keys = array_keys($array);
        for($i=0; $i < count($array); ++$i) {
            if ($key == $keys[$i]) {
                return true;
            }
        }
        return false;
    }
    
    echo "\nSearch in keys\n";
    $key = "Italy";
    if (searchInKeys($populationInMillion, $key)) {
        echo "Key exits";
    } else {
        echo "Key does not exist";
    }
    
    // Seacrh in values
    function searchInValues ($array, $val) {
        $keys = array_keys($array);
        for($i=0; $i < count($array); ++$i) {
            if ($val == $array[$keys[$i]]) {
                return true;
            }
        }
        return false;
    }
    
    echo "\n\nSearch in values\n";
    $val = 59;
    if (searchInValues($populationInMillion, $val)) {
        echo "Value exits";
    } else {
        echo "Value does not exist";
    }
    
    // foo function: Key-value pairs
    echo "\n\nKey-value pairs:\n";
    function foo($array) {
        foreach ($array as $country => $population) { 
            echo $country . " => " . $population . "\n"; 
        } 
    }
    
    foo ($populationInMillion);
    
    // Clear an array
    unset ($populationInMillion);
    $populationInMillion = array();
    
    // Print cleared array
    echo "\nArray is cleared:\n";
    print_r ($populationInMillion);
    
    // Nested associative arrays
    $populationInMillion = array(
        "Europa" => array("Italy" => 59, "France" => 67.5),
        "Asia" => array("Turkey" => 85, "Beijing" => 21.5)
    );
    
    echo "\nNested associative array:\n";
    print_r ($populationInMillion);
    
    // Access nested elemetns
    echo "\nPopulation of Beijing is " . $populationInMillion["Asia"]['Beijing'] 
        . " millions in 2022";
        
    // Modify nested elemets
    $populationInMillion["Europa"]["France"] = 70;
    
    // Remove an element
    unset ($populationInMillion["Europa"]["Italy"]);
    
    // Print updated nested array
    echo "\n\nUpdated nested array:\n";
    print_r ($populationInMillion);
    
    // Romove an inner array from the nested one
    unset ($populationInMillion["Europa"]);
    
    // Print the final array
    echo "\nThe final nested array:\n";
    print_r ($populationInMillion);
    
    // Clear the array
    unset ($populationInMillion);
    $populationInMillion = null;
    
?>
