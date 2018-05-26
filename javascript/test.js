function getQueryVars()
{
    // aray for return value
    var vars = [];
    // get query char and divide "&"
    var query_list = window.location.search.substring(1).split('&');
    // TEMP value for get value
    var temp_arr;
    //
    query_list.forEach(function(e, i, a){
        temp_arr = e.split('=');
        vars[ temp_arr[0] ] = temp_arr[1];
    })
    return vars;
}
