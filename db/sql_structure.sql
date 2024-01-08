CREATE TABLE dict (
    id INTEGER PRIMARY KEY,
    question TEXT NOT NULL,
    response TEXT NOT NULL,
    isCheckedQuestion BOOLEAN NOT NULL DEFAULT 0,
    isCheckedResponse BOOLEAN NOT NULL DEFAULT 0
);
