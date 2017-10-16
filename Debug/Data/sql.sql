DROP TABLE data;
CREATE TABLE data (id INTEGER PRIMARY KEY AUTOINCREMENT, lattice INTEGER, a FLOAT, b FLOAT, c FLOAT, alpha FLOAT, beta FLOAT, gamma FLOAT, attach TEXT, remark1 TEXT, remark2 TEXT, remark3 TEXT, remark4 TEXT, remark5 TEXT);
INSERT INTO data (lattice, a, b, c, alpha, beta, gamma, attach, remark1, remark2, remark3, remark4, remark5) VALUES (1, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, "value中文", "", "", "", "", "");
UPDATE data SET remark1 = "remark" WHERE id = 1;
SELECT id, lattice, a, b, c, alpha, beta, gamma, attach, remark1, remark2, remark3, remark4, remark5 FROM data;