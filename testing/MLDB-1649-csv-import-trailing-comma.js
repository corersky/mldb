// This file is part of MLDB. Copyright 2016 mldb.ai inc. All rights reserved.

var mldb = require('mldb')
var unittest = require('mldb/unittest')

// Contents of fixture:
// a,b,c.a,c.b,"""d.a"""
// 1,2,3,4,5


var importConfig = {
    type: 'import.text',
    params: {
        dataFileUrl: "file://mldb/testing/dataset/MLDB-1649.csv",
        runOnCreation: true,
        outputDataset: 'mldb1649',
        structuredColumnNames: false
    }
};

var resp = mldb.put('/v1/procedures/import', importConfig);

mldb.log(resp);

unittest.assertEqual(resp.responseCode, 201);

var resp = mldb.get('/v1/query', { q: 'select * from mldb1649', format: 'table' });

mldb.log(resp.json);

var expected = [
   [ "_rowName", "a", "b", "c" ],
   [ "2", 1, 2, 3 ]
];

unittest.assertEqual(resp.json, expected);

"success"

