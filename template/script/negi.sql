--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

--
-- Name: config_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE config_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.config_id_seq OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: config; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE config (
    name text,
    value text,
    id integer DEFAULT nextval('config_id_seq'::regclass) NOT NULL
);


ALTER TABLE public.config OWNER TO postgres;

--
-- Name: error_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE error_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.error_id_seq OWNER TO postgres;

--
-- Name: error; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE error (
    id integer DEFAULT nextval('error_id_seq'::regclass) NOT NULL,
    error text,
    "timestamp" timestamp without time zone DEFAULT now()
);


ALTER TABLE public.error OWNER TO postgres;

--
-- Name: regexp_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE regexp_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.regexp_id_seq OWNER TO postgres;

--
-- Name: rule; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
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


ALTER TABLE public.rule OWNER TO postgres;

--
-- Name: save_data_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE save_data_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
    CYCLE;


ALTER TABLE public.save_data_id_seq OWNER TO postgres;

--
-- Name: save_history; Type: TABLE; Schema: public; Owner: test; Tablespace: 
--

CREATE TABLE save_history (
    "timestamp" timestamp without time zone,
    src_mac_addr text,
    url text,
    title text,
    browsing_time text
);


ALTER TABLE public.save_history OWNER TO test;

--
-- Name: save_packet_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE save_packet_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
    CYCLE;


ALTER TABLE public.save_packet_id_seq OWNER TO postgres;

--
-- Name: save_packet; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
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


ALTER TABLE public.save_packet OWNER TO postgres;

--
-- Name: save_result_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE save_result_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
    CYCLE;


ALTER TABLE public.save_result_id_seq OWNER TO postgres;

--
-- Name: save_result; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE save_result (
    id integer DEFAULT nextval('save_result_id_seq'::regclass) NOT NULL,
    stream_id integer,
    rule_id integer,
    pattern text,
    pattern_len integer,
    place integer,
    result text,
    "timestamp" timestamp without time zone,
    src_ip text,
    dst_ip text,
    src_port integer,
    dst_port integer,
    src_mac_addr text,
    dst_mac_addr text
);


ALTER TABLE public.save_result OWNER TO postgres;

--
-- Name: save_stream; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
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


ALTER TABLE public.save_stream OWNER TO postgres;

--
-- Name: update_check; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE update_check (
    value text,
    updated integer
);


ALTER TABLE public.update_check OWNER TO postgres;

--
-- Data for Name: config; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY config (name, value, id) FROM stdin;
stream_buf_size	10240	1
http_max_body_size	20480	2
emergency_save_all	1	3
\.


--
-- Name: config_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('config_id_seq', 3, true);


--
-- Data for Name: error; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY error (id, error, "timestamp") FROM stdin;
\.


--
-- Name: error_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('error_id_seq', 24, true);


--
-- Name: regexp_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('regexp_id_seq', 484, true);


--
-- Data for Name: rule; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY rule (id, owner, expire_date, src_ip, src_netmask, src_port, dst_ip, dst_netmask, dst_port, "timestamp", regexp, prefilter_pattern, prefilter_offset, prefilter_depth, save_flag) FROM stdin;
481	hoge	2011-09-11	\N	\N	\N	\N	\N	\N	2011-09-11 21:36:39.451221	\N	html	0	0	1
1	hoge	2011-01-11	\N	\N	\N	\N	\N	\N	2011-01-11 14:40:36.136482	\N	GET	0	0	1
2	hoge	2011-01-11	\N	\N	\N	\N	\N	\N	2011-01-11 14:40:36.136482	\N	Host:	0	0	1
3	hoge	2011-01-11	\N	\N	\N	\N	\N	\N	2011-01-11 14:40:36.136482	\N	Referer:	0	0	1
4	hoge	2011-01-11	\N	\N	\N	\N	\N	\N	2011-01-11 14:40:36.136482	\N	Cookie:	0	0	1
7	test	2014-08-07	\N	\N	\N	\N	\N	\N	2014-08-07 15:52:33.189141	\N	<h1	0	0	1
8	test	2014-08-07	\N	\N	\N	\N	\N	\N	2014-08-07 15:53:07.094857	\N	<h2	0	0	1
9	test	2014-08-07	\N	\N	\N	\N	\N	\N	2014-08-07 15:53:37.645747	\N	<h3	0	0	1
10	test	2014-08-20	\N	\N	\N	\N	\N	\N	2014-08-20 15:51:19.965455	\N	<TITLE	0	0	1
6	hoge	2011-09-11	\N	\N	\N	\N	\N	\N	2011-09-11 21:36:39.451221	\N	<title	0	0	1
11	test	2014-08-20	\N	\N	\N	\N	\N	\N	2014-08-20 17:23:15.175425	\N	User-Agent:	0	0	1
12	test	2014-08-20	\N	\N	\N	\N	\N	\N	2014-08-20 17:24:26.937239	\N	Content-Type:	0	0	1
5	hoge	2011-09-11	\N	\N	\N	\N	\N	\N	2011-09-11 21:36:39.451221	\N	Set-Cookie:	0	0	1
\.


--
-- Name: save_data_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('save_data_id_seq', 1, true);


--
-- Data for Name: save_history; Type: TABLE DATA; Schema: public; Owner: test
--

COPY save_history ("timestamp", src_mac_addr, url, title, browsing_time) FROM stdin;
\.


--
-- Data for Name: save_packet; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY save_packet ("timestamp", src_ip, dst_ip, src_port, dst_port, packet_size, packet_size_org, content_size, error, content, id, protocol, flag) FROM stdin;
\.


--
-- Name: save_packet_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('save_packet_id_seq', 1, false);


--
-- Data for Name: save_result; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY save_result (id, stream_id, rule_id, pattern, pattern_len, place, result, "timestamp", src_ip, dst_ip, src_port, dst_port, src_mac_addr, dst_mac_addr) FROM stdin;
\.


--
-- Name: save_result_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('save_result_id_seq', 1, true);


--
-- Data for Name: save_stream; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY save_stream (id, "timestamp", src_ip, dst_ip, src_port, dst_port, protcol, direction, truncate, regexp_id, stream, st_size, st_size_org, l5prot, match_str, http_size, http_header_size, http_chunked, http_compress, error, stream_org, srim_version, hit_count, after_ipfilter, after_prefilter, prefilter_log, l7_error, l7prot) FROM stdin;
\.


--
-- Data for Name: update_check; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY update_check (value, updated) FROM stdin;
regexp	0
filter	0
\.


--
-- Name: config_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY config
    ADD CONSTRAINT config_pkey PRIMARY KEY (id);


--
-- Name: regexp_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY rule
    ADD CONSTRAINT regexp_pkey PRIMARY KEY (id);


--
-- Name: save_data_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
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

