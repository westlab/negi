--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

--
-- Name: config_id_seq; Type: SEQUENCE; Schema: public; 
--

CREATE SEQUENCE config_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;



--
-- Name: config_id_seq; Type: SEQUENCE SET; Schema: public; 
--

SELECT pg_catalog.setval('config_id_seq', 3, true);


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: config; Type: TABLE; Schema: public; Tablespace: 
--

CREATE TABLE config (
    name text,
    value text,
    id integer DEFAULT nextval('config_id_seq'::regclass) NOT NULL
);



--
-- Name: error_id_seq; Type: SEQUENCE; Schema: public; 
--

CREATE SEQUENCE error_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;



--
-- Name: error_id_seq; Type: SEQUENCE SET; Schema: public; 
--

SELECT pg_catalog.setval('error_id_seq', 24, true);


--
-- Name: error; Type: TABLE; Schema: public; Tablespace: 
--

CREATE TABLE error (
    id integer DEFAULT nextval('error_id_seq'::regclass) NOT NULL,
    error text,
    "timestamp" timestamp without time zone DEFAULT now()
);



--
-- Name: regexp_id_seq; Type: SEQUENCE; Schema: public; 
--

CREATE SEQUENCE regexp_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;



--
-- Name: regexp_id_seq; Type: SEQUENCE SET; Schema: public; 
--

SELECT pg_catalog.setval('regexp_id_seq', 482, true);


--
-- Name: rule; Type: TABLE; Schema: public; Tablespace: 
--

CREATE TABLE rule (
    id integer DEFAULT nextval('regexp_id_seq'::regclass) NOT NULL,
    owner text,
    expire_date date DEFAULT now(),
    src_ip text,
    src_netmask text,
    src_port integer,
    dst_ip text,
    dst_netmask text,
    dst_port integer,
    "timestamp" timestamp without time zone DEFAULT now(),
    regexp text,
    prefilter_pattern text,
    prefilter_offset integer,
    prefilter_depth integer,
    save_flag integer
);



--
-- Name: save_data_id_seq; Type: SEQUENCE; Schema: public; 
--

CREATE SEQUENCE save_data_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1
    CYCLE;



--
-- Name: save_data_id_seq; Type: SEQUENCE SET; Schema: public; 
--

SELECT pg_catalog.setval('save_data_id_seq', 1, true);


--
-- Name: save_packet_id_seq; Type: SEQUENCE; Schema: public; 
--

CREATE SEQUENCE save_packet_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1
    CYCLE;



--
-- Name: save_packet_id_seq; Type: SEQUENCE SET; Schema: public; 
--

SELECT pg_catalog.setval('save_packet_id_seq', 1, false);


--
-- Name: save_packet; Type: TABLE; Schema: public; Tablespace: 
--

CREATE TABLE save_packet (
    "timestamp" timestamp without time zone,
    src_ip text,
    dst_ip text,
    src_port integer,
    dst_port integer,
    packet_size integer,
    packet_size_org integer,
    content_size integer,
    error text,
    content bytea,
    id integer DEFAULT nextval('save_packet_id_seq'::regclass) NOT NULL,
    protocol integer,
    flag text
);



--
-- Name: save_result_id_seq; Type: SEQUENCE; Schema: public; 
--

CREATE SEQUENCE save_result_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1
    CYCLE;



--
-- Name: save_result_id_seq; Type: SEQUENCE SET; Schema: public; 
--

SELECT pg_catalog.setval('save_result_id_seq', 3909, true);


--
-- Name: save_result; Type: TABLE; Schema: public; Tablespace: 
--

CREATE TABLE save_result (
    id integer DEFAULT nextval('save_result_id_seq'::regclass) NOT NULL,
    stream_id integer,
    rule_id integer,
    pattern text,
    pattern_len integer,
    place integer,
    result bytea
);



--
-- Name: save_stream; Type: TABLE; Schema: public; Tablespace: 
--

CREATE TABLE save_stream (
    id integer DEFAULT nextval('save_data_id_seq'::regclass) NOT NULL,
    "timestamp" timestamp without time zone,
    src_ip text,
    dst_ip text,
    src_port integer,
    dst_port integer,
    protcol integer,
    direction integer,
    truncate integer,
    regexp_id text,
    stream bytea,
    st_size integer,
    st_size_org integer,
    l5prot integer,
    match_str bytea,
    http_size text,
    http_header_size text,
    http_chunked text,
    http_compress text,
    error text,
    stream_org bytea,
    srim_version text,
    hit_count integer,
    after_ipfilter integer DEFAULT (-1),
    after_prefilter integer DEFAULT (-1),
    prefilter_log text,
    l7_error integer,
    l7prot integer
);



--
-- Name: update_check; Type: TABLE; Schema: public; Tablespace: 
--

CREATE TABLE update_check (
    value text,
    updated integer
);



--
-- Data for Name: config; Type: TABLE DATA; Schema: public; 
--

COPY config (name, value, id) FROM stdin;
stream_buf_size	10240	1
http_max_body_size	20480	2
emergency_save_all	1	3
\.


--
-- Data for Name: error; Type: TABLE DATA; Schema: public; 
--

COPY error (id, error, "timestamp") FROM stdin;
1	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
2	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
3	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
4	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
5	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
6	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
7	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
8	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
9	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
10	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
11	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
12	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
13	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
14	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
15	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
16	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
17	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
18	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
19	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
20	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
21	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
22	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
23	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
24	regexp_syntax[0]: Empty expression	2009-05-28 21:38:22.943183
\.


--
-- Data for Name: rule; Type: TABLE DATA; Schema: public; 
--

COPY rule (id, owner, expire_date, src_ip, src_netmask, src_port, dst_ip, dst_netmask, dst_port, "timestamp", regexp, prefilter_pattern, prefilter_offset, prefilter_depth, save_flag) FROM stdin;
478	hoge	2011-01-11	\N	\N	\N	\N	\N	\N	2011-01-11 14:40:36.136482	\N	html	0	0	1
481	hoge	2011-09-11	\N	\N	\N	\N	\N	\N	2011-09-11 21:36:39.451221	\N	<a 	0	0	1
\.


--
-- Data for Name: save_packet; Type: TABLE DATA; Schema: public; 
--

COPY save_packet ("timestamp", src_ip, dst_ip, src_port, dst_port, packet_size, packet_size_org, content_size, error, content, id, protocol, flag) FROM stdin;
\.


--
-- Data for Name: save_result; Type: TABLE DATA; Schema: public; 
--

COPY save_result (id, stream_id, rule_id, pattern, pattern_len, place, result) FROM stdin;
\.


--
-- Data for Name: save_stream; Type: TABLE DATA; Schema: public; 
--

COPY save_stream (id, "timestamp", src_ip, dst_ip, src_port, dst_port, protcol, direction, truncate, regexp_id, stream, st_size, st_size_org, l5prot, match_str, http_size, http_header_size, http_chunked, http_compress, error, stream_org, srim_version, hit_count, after_ipfilter, after_prefilter, prefilter_log, l7_error, l7prot) FROM stdin;
\.


--
-- Data for Name: update_check; Type: TABLE DATA; Schema: public; 
--

COPY update_check (value, updated) FROM stdin;
regexp	0
filter	0
\.


--
-- Name: config_pkey; Type: CONSTRAINT; Schema: public; Tablespace: 
--

ALTER TABLE ONLY config
    ADD CONSTRAINT config_pkey PRIMARY KEY (id);


--
-- Name: regexp_pkey; Type: CONSTRAINT; Schema: public; Tablespace: 
--

ALTER TABLE ONLY rule
    ADD CONSTRAINT regexp_pkey PRIMARY KEY (id);


--
-- Name: save_data_pkey; Type: CONSTRAINT; Schema: public; Tablespace: 
--

ALTER TABLE ONLY save_stream
    ADD CONSTRAINT save_data_pkey PRIMARY KEY (id);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

