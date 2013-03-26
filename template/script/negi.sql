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
    result bytea,
    "timestamp" timestamp without time zone,
    src_ip text,
    dst_ip text,
    src_port integer,
    dst_port integer
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

