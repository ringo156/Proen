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

function readJSON(id, filename)
{
    $.getJSON(filename)
        .done(function(data){
            // console.log(id);
            // console.log(data[id].title);
            var url = data[id].title + ".html";
            // console.log(url);
            return url;
    })
}
